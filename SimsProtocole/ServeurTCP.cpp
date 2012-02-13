#include "utils.h"
#include "ServeurTCP.h"
#include <QMessageBox>

ServeurTCP::ServeurTCP(QObject *parent) :
    QObject(parent)
{

    if (!StartServer())
    {
        QMessageBox::information(0,"Serveur TCP","Serveur non démarré",QMessageBox::Ok);
    }
}



bool ServeurTCP::ServerStarted()
{
    return serverStarted;
}


bool ServeurTCP::StartServer()
{
    // Gestion du serveur
    serveur = new QTcpServer(this);
    if (!serveur->listen(QHostAddress::Any, PORT_SERVEUR)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        serverStarted = false;
    }
    else
    {
        serverStarted = true;
        connect(serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }
    return serverStarted;
}


void ServeurTCP::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = serveur->nextPendingConnection();
    emit AjouterClient(nouveauClient);
}
