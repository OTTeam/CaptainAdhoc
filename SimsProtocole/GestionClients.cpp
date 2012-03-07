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


    _clients.clear();
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
    //    qDebug()<< "********************************************" ;
    //    qDebug()<< "newConnectionRequest" ;


    //    qDebug()<< "routes : " << routes.size();

    qDebug() << "BroadCasted by" << broadcasterAddress.toString() << "(" << routes.size() << "hosts)";

    Client *broadcasterClient = NULL;
    // première étape : vérifier si l'envoyeur du broadcast est nouveau ou pas.
    bool broadCasterExists = false;
    foreach (Client *client, _clients)
    {
        if (client->address() == broadcasterAddress)
        {
            broadCasterExists = true;

            broadcasterClient = client;
            if (broadcasterClient->hopNumber()>1)
            {
                _clients.removeOne(broadcasterClient);
                broadcasterClient->deleteLater();
                broadCasterExists = false;
                qDebug() << "Replacing Broadcaster...";
            }
            break;
        }
    }

    // qDebug() << "broadCasterExists :" << broadCasterExists;

    //s'il nexiste pas, on le crée (on ne connecte pas le socket tout de suite, afin de pouvoir ajouter les autres entre temps
    if (broadCasterExists == false)
    {
        qDebug() << "Client discovered - Broadcaster";
        broadcasterClient = new Client(broadcasterAddress);
        connect(broadcasterClient, SIGNAL(Connected()), this, SLOT(clientConnected()));
        connect(broadcasterClient, SIGNAL(SocketError()), this, SLOT(clientConnectionFailed()));

    }

    // ensuite, on rajoute toutes les routes en mettant le noeud en nextHop
    foreach(RoutesTableElt newRoute, routes)
    {

        //        qDebug()<< newRoute.destAddr << " hopNb :" << newRoute.hopNumber;

        bool routeExists= false;
        //qDebug()<< "route :" << newRoute.destAddr.toString() << "hop :" << newRoute.hopNumber;
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
                    //qDebug() << "updateRoute";
                    client->UpdateRoute(broadcasterClient->socket(), broadcasterAddress, newRoute.hopNumber);
                }
                break;
            }
        }

        //qDebug() << "routeExists :" << routeExists;
        // si c'est une nouvelle route, on rajoute le client.
        if (routeExists == false)
        {
            qDebug() << "Client discovered - Broadcaster's client";

            Client *client = new Client(broadcasterClient->socket(),newRoute.destAddr,broadcasterAddress, newRoute.hopNumber);
            connect(client, SIGNAL(Connected()), this, SLOT(clientConnected()));
            connect(client, SIGNAL(SocketError()), this, SLOT(clientConnectionFailed()));

            if (broadCasterExists)
                NewClientConfig(client);
        }
    }
    // si on a créé un nouveau broadcaster, on doit connecter son socket.
    if ( broadCasterExists == false )
        broadcasterClient->connectSocket();


    //    qDebug()<< "********************************************" ;
}


void GestionClients::newConnectionDone(QTcpSocket *socket)
{

    foreach (Client *client, _clients)
    {
        // si la nouvele route est connue
        if (client->address() == socket->peerAddress())
        {
            // si la nouvelle route est meilleure, on change celle du client
            if (client->hopNumber() > 1)
            {
                qDebug() << "Client updated - TCP connect";;
                client->UpdateRoute(socket, socket->peerAddress(), 1);
                connect(socket,SIGNAL(readyRead()),this,SLOT(clientBytesAvailable()));
                connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(clientBytesWritten(qint64)));


            }
            return;
        }
    }

    qDebug() << "Client discovered - TCP connect";

    Client *client = new Client(socket);
    connect(client, SIGNAL(Connected()),this,SLOT(clientConnected()));
    NewClientConfig(client);
}


void GestionClients::clientConnected()
{
    Client *client = (Client *) sender();

    NewClientConfig(client);
}

void GestionClients::NewClientConfig(Client *client)
{
    connect(client,   SIGNAL(Disconnected())       , this, SLOT(clientDisconnect()));
    disconnect(client,SIGNAL(SocketError()),this,SLOT(clientConnectionFailed()));
    connect(client,SIGNAL(DownloadSpeedUpdate(int)), this, SLOT(downloadSpeedUpdate(int)));
    connect(client,SIGNAL(UploadSpeedUpdate(int))  , this, SLOT(uploadSpeedUpdate(int)));
    connect(client,SIGNAL(BytesReceivedUpdate(int)), this, SLOT(clientReceived(int)));
    connect(client,SIGNAL(BytesSentUpdate(int))    , this, SLOT(clientSent(int)));

    if (client->hopNumber() == 1 )
    {
        connect(client->socket(),SIGNAL(readyRead()),this,SLOT(clientBytesAvailable()));
        connect(client->socket(),SIGNAL(bytesWritten(qint64)),this,SLOT(clientBytesWritten(qint64)));
    }

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
    client->deleteLater();

}

void GestionClients::clientDisconnect()
{
    Client *client = (Client *)sender();

    SocketsHandlers *socketHandler = findSocketHandler(client->socket());
    if ( socketHandler != NULL)
        _socketsHandlers.removeOne(socketHandler);

    _clients.removeOne(client);

    client->deleteLater();


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
        if (socketHandler->paquetSize == 0)
        {
            if (senderSocket->bytesAvailable() < sizeof(quint16))
                return;

            in >> socketHandler->paquetSize;
        }
        // si le paquet n'est pas entier, on passe
        if (senderSocket->bytesAvailable() < socketHandler->paquetSize)
            return;

        // sinon, on lit les adresses de source et Destination pour savoir ce qu'il faut en faire
        QString destAddStr;
        QString senderAddStr;

        in >> destAddStr;
        in >> senderAddStr;

        destAdd = destAddStr;
        senderAdd = senderAddStr;

        //        qDebug() << destAddStr;

        //        qDebug() << senderAddStr;

        //        qDebug() << senderSocket->localAddress();

        socketHandler->paquetSize = 0;
        // si nous sommes l'objectif, on appelle le bon client pour qu'il lise le paquet
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

            quint16 dataSize;
            QByteArray data;

            in >> dataSize;
            data.resize(dataSize);
            in.readRawData(data.data(),dataSize);


            client->ForwardMessage(senderAdd, destAdd, data);
        }



    }

}

void GestionClients::clientBytesWritten(qint64 bytesWritten)
{
    Q_UNUSED(bytesWritten)

    // on lit l'en-tête du paquet afin de savoir pour quel client il est destiné :
    QTcpSocket *senderSocket = (QTcpSocket *) sender();

    Client *client = findClientByDest(senderSocket->peerAddress());

    client->newBytesWritten(bytesWritten);
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
