#include "GestionClients.h"
#include <QMessageBox>

GestionClients::GestionClients(QObject *parent) :
    QObject(parent)
{
    serveurEcoute = new ServeurTCP(this);
    connect(serveurEcoute,SIGNAL(AjouterClient(QTcpSocket*)),this,SLOT(newConnectionDone(QTcpSocket*)));

}


void GestionClients::clientReceived(int percentComplete)
{
    //Client *clientSender = (Client *)sender();
    //QMessageBox::information(this,"progress",QString::number(Progress),QMessageBox::Ok);
    emit TransfertUpdate(percentComplete);
}


void GestionClients::newConnectionRequest(QString address)
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
    clients.push_back(client);
    emit ClientNumberChanged(clients.size());
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
