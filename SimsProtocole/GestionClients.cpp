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
    Client *clientSender = (Client *)sender();
    emit TransfertUpdate(percentComplete);


}


void GestionClients::newConnectionRequest(QString address)
{
    QMessageBox::information(0,"connexion à" ,address,QMessageBox::Ok);
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
        QMessageBox::information(0,"NewConnexionREquest" ,"Ajout d'un nouveau client",QMessageBox::Ok);
        Client *client = new Client(address);
        clients.push_back(client);
        connect(client,SIGNAL(NewData(int)),this,SLOT(clientReceived(int)));
    }

}
void GestionClients::newConnectionDone(QTcpSocket *socket)
{
    Client *client = new Client(socket);
    connect(client,SIGNAL(NewData(int)),this,SLOT(clientReceived(int)));
    clients.push_back(client);
}


void GestionClients::clientDisconnect()
{
    Client *client = (Client *)sender();
    clients.removeOne(client);
}

void GestionClients::sendToAll()
{

    foreach (Client *client, clients)
    {
        client->sendMessage(QString("Hello!"));
    }


}
