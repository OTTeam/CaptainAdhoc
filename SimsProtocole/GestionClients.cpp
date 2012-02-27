#include "GestionClients.h"
#include <QMessageBox>

GestionClients::GestionClients(QObject *parent) :
    QObject(parent)
{
    listeningServer = new TCPServer(this);
    connect(listeningServer,SIGNAL(AjouterClient(QTcpSocket*)),this,SLOT(newConnectionDone(QTcpSocket*)));
    clientDiscoveryModule = new ClientDiscovery(this);
    connect(clientDiscoveryModule,SIGNAL(DatagramReceived(QHostAddress)),this,SLOT(newConnectionRequest(QHostAddress)));
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


void GestionClients::newConnectionRequest(QHostAddress address)
{

    bool clientExists = false;
    foreach (Client *client, clients)
    {
        if (client->getAddress() == address)
        {
            clientExists = true;
            break;
        }
    }

    if (clientExists == false)
    {
        Client *client = new Client(address);
        connect(client, SIGNAL(connected(Client *)), this, SLOT(clientConnected(Client *)));
    }
}


void GestionClients::newConnectionDone(QTcpSocket *socket)
{
    Client *client = new Client(socket);
    clientConnected(client);
    connect(client, SIGNAL(connected(Client *)),this,SLOT(clientConnected(Client *)));
}


void GestionClients::clientConnected(Client *client)
{
    connect(client,SIGNAL(BytesReceivedUpdate(int)), this, SLOT(clientReceived(int)));
    connect(client,SIGNAL(BytesSentUpdate(int))    , this, SLOT(clientSent(int)));
    connect(client,SIGNAL(disconnected())          , this, SLOT(clientDisconnect()));
    connect(client,SIGNAL(DownloadSpeedUpdate(int)), this, SLOT(downloadSpeedUpdate(int)));
    connect(client,SIGNAL(UploadSpeedUpdate(int))  , this, SLOT(uploadSpeedUpdate(int)));

    clients.push_back(client);

    emit ClientNumberChanged(clients.size());
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


void GestionClients::clientDisconnect()
{
    Client *client = (Client *)sender();
    clients.removeOne(client);
    emit ClientNumberChanged(clients.size());
}

void GestionClients::sendToAll()
{
    foreach (Client *client, clients)
    {
        client->sendMessage();
    }
}
