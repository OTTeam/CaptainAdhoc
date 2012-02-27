#include "GestionClients.h"
#include <QMessageBox>

GestionClients::GestionClients(QObject *parent) :
    QObject(parent)
{
    serveurEcoute = new TCPServer(this);
    connect(serveurEcoute,SIGNAL(AjouterClient(QTcpSocket*)),this,SLOT(newConnectionDone(QTcpSocket*)));
    clientDiscoveryModule = new ClientDiscovery(this);
    connect(clientDiscoveryModule,SIGNAL(DatagramReceived(QHostAddress)),this,SLOT(newConnectionRequest(QHostAddress)));
}


void GestionClients::clientReceived(int percentComplete)
{
    emit TransfertUpdate(percentComplete);
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
    connect(client,SIGNAL(NewData(int)),this,SLOT(clientReceived(int)));
    connect(client,SIGNAL(disconnected()),this,SLOT(clientDisconnect()));
    connect(client,SIGNAL(NetworkSpeedUpdate(int)),this,SLOT(dlSpeedUpdate(int)));
    clients.push_back(client);
    emit ClientNumberChanged(clients.size());
}

void GestionClients::dlSpeedUpdate(int bytesPerSec)
{
    emit NetworkSpeedUpdate(bytesPerSec);
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
