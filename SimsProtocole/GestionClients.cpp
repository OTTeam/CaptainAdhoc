#include "GestionClients.h"
#include <QMessageBox>

GestionClients::GestionClients(QObject *parent) :
    QObject(parent)
{
    _listeningServer = new TCPServer(this);
    connect(_listeningServer,SIGNAL(AjouterClient(QTcpSocket*)),this,SLOT(newConnectionDone(QTcpSocket*)));
    _clientDiscoveryModule = new ClientDiscovery(this);
    connect(_clientDiscoveryModule,SIGNAL(DatagramReceived(QHostAddress,QList<RoutesTableElt>)),this,SLOT(newConnectionRequest(QHostAddress,QList<RoutesTableElt>)));

    // Gestion de la découverte du réseau
    _timerBroadcast = new QTimer(this);

    _timerBroadcast->setInterval(BROADCAST_INTERVAL);
    _timerBroadcast->setSingleShot(false);
    connect(_timerBroadcast,SIGNAL(timeout()),this,SLOT(broadCastTrigger()));
    _timerBroadcast->start();



 }


void GestionClients::clientReceived(int percentComplete)
{
    Client *client = (Client *) sender();

    emit ClientDownloadUpdate(client, percentComplete);
}

void GestionClients::clientSent(int percentComplete)
{
    Client *client = (Client *) sender();

    emit ClientUploadUpdate(client, percentComplete);
}


void GestionClients::newConnectionRequest(QHostAddress broadcasterAddress,QList<RoutesTableElt> routes)
{
    Client *broadcasterClient;
    // première étape : vérifier si l'envoyeur du broadcast est nouveau ou pas.
    bool broadCasterExists = false;
    foreach (Client *client, _clients)
    {
        if (client->socket()->peerAddress() == broadcasterAddress)
        {
            broadCasterExists = true;
            broadcasterClient = client;
            break;
        }
    }
    //s'il nexiste pas, on le crée (on ne connecte pas le socket tout de suite, afin de pouvoir ajouter les autres entre temps
    if (broadCasterExists == false)
    {
        broadcasterClient = new Client(broadcasterAddress);
        connect(broadcasterClient, SIGNAL(Connected()), this, SLOT(clientConnected()));
        connect(broadcasterClient, SIGNAL(Disconnected()), this, SLOT(clientConnectionFailed()));
    }

    // ensuite, on rajoute toutes les routes en mettant le noeud en nextHop
    foreach(RoutesTableElt newRoute, routes)
    {
        bool routeExists;
        //parcourt des clients pour retrouver la route.
        foreach (Client *client, _clients)
        {
            // si la nouvele route est connue
            if (client->address() == newRoute.destAddr)
            {
                routeExists = true;
                // si la nouvelle route est meilleure, on change celle du client
                if (client->hopNumber() > newRoute.hopNumber)
                {
                    client->UpdateRoute(broadcasterClient->socket(), newRoute.hopNumber);
                }
                break;
            }
        }
        // si c'est une nouvelle route, on rajoute le client.
        if (!routeExists)
        {
            Client *client = new Client(broadcasterClient->socket(),newRoute.destAddr,newRoute.hopNumber);
            connect(client, SIGNAL(Connected()), this, SLOT(clientConnected()));
            connect(client, SIGNAL(Disconnected()), this, SLOT(clientConnectionFailed()));
        }
    }
    // si on a créé un nouveau broadcaster, on doit connecter son socket.
    if ( !broadCasterExists)
        broadcasterClient->connectSocket();

}


void GestionClients::newConnectionDone(QTcpSocket *socket)
{
    Client *client = new Client(socket);
    connect(client, SIGNAL(Connected()),this,SLOT(clientConnected()));
}


void GestionClients::clientConnected()
{
    Client *client = (Client *) sender();
    connect(client,SIGNAL(BytesReceivedUpdate(int)), this, SLOT(clientReceived(int)));
    connect(client,SIGNAL(BytesSentUpdate(int))    , this, SLOT(clientSent(int)));
    connect(client,SIGNAL(Disconnected())          , this, SLOT(clientDisconnect()));
    connect(client,SIGNAL(DownloadSpeedUpdate(int)), this, SLOT(downloadSpeedUpdate(int)));
    connect(client,SIGNAL(UploadSpeedUpdate(int))  , this, SLOT(uploadSpeedUpdate(int)));

    connect(client->socket(),SIGNAL(readyRead()),this,SLOT(clientBytesAvailable()));
    connect(client->socket(),SIGNAL(bytesWritten(qint64)),this,SLOT(clientBytesWritten(qint64)));
    _clients.push_back(client);

    // si c'est un nouveau socket, on le rajoute dans notre table de référence.
    if (findSocketHandler(client->socket()) == NULL)
    {
        SocketsHandlers *newSocketHandler  = new SocketsHandlers;
        newSocketHandler->socket = client->socket();
        newSocketHandler->paquetSize = 0;
        _socketsHandlers.push_back(newSocketHandler);
    }
    emit ClientNumberChanged(_clients.size());
}


void GestionClients::uploadSpeedUpdate(int bytesPerSec)
{
    Client *client = (Client *) sender();

    emit ClientUploadSpeedUpdate(client, bytesPerSec);
}


void GestionClients::downloadSpeedUpdate(int bytesPerSec)
{
    Client *client = (Client *) sender();

    emit ClientUploadSpeedUpdate(client, bytesPerSec);
}

void GestionClients::broadCastTrigger()
{
    _clientDiscoveryModule->sendNewDatagram(_clients);
}


void GestionClients::clientConnectionFailed()
{
    Client *client = (Client *)sender();
    //on détruit juste le client
    delete client;

}

void GestionClients::clientDisconnect()
{
    Client *client = (Client *)sender();

    _clients.removeOne(client);

    SocketsHandlers *socketHandler = findSocketHandler(client->socket());
    if ( socketHandler != NULL)
        _socketsHandlers.removeOne(socketHandler);

    emit ClientNumberChanged(_clients.size());
}

void GestionClients::sendToAll()
{
    foreach (Client *client, _clients)
    {
        client->sendMessage();
    }
}


void GestionClients::clientBytesAvailable()
{
    // on lit l'en-tête du paquet afin de savoir pour quel client il est destiné :
    QTcpSocket *senderSocket = (QTcpSocket *) sender();

    SocketsHandlers *socketHandler = findSocketHandler(senderSocket);


    QDataStream in(senderSocket);

    QHostAddress destAdd;
    QHostAddress senderAdd;

    while (senderSocket->bytesAvailable() > 0)
    {
        if (socketHandler->paquetSize == 0 && senderSocket->bytesAvailable() < sizeof(quint16))
            return;

        in >> socketHandler->paquetSize;



        // si le paquet n'est pas entier, on passe
        if (senderSocket->bytesAvailable() < socketHandler->paquetSize)
            return;

        // sinon, on lit les adresses de source et Destination pour savoir ce qu'il faut en faire

        in >> destAdd;
        in >> senderAdd;

        // si nous sommes l'objectif, on appelle le bon client pour qu'il lise le paquet
        //
        if (destAdd == senderSocket->localAddress())
        {
            // on trouve le client connecté à l'adresse de l'envoyeur.
            Client *client = findClientByDest(senderAdd);
            // on appelle son slot de réception de données.
            client->newBytesReceived();
        }
        else
        {
            // le paquet ne nous est pas destiné. on récupère donc la passerelle et on lui demande de faire suivre
            Client *client = findClientByDest(destAdd);
            client->ForwardMessage(senderAdd, destAdd);
        }



    }

}

void GestionClients::clientBytesWritten(qint64 bytesWritten)
{
    Q_UNUSED(bytesWritten)

}


Client *GestionClients::findClientByDest(QHostAddress destAddress)
{
    foreach(Client *client, _clients)
    {
        if (client->address() == destAddress)
            return client;
    }
    return NULL;
}

SocketsHandlers *GestionClients::findSocketHandler(QTcpSocket *socketSearched)
{
    foreach(SocketsHandlers *socketHandler, _socketsHandlers)
    {
        if (socketSearched == socketHandler->socket)
            return socketHandler;
    }
    return NULL;
}
