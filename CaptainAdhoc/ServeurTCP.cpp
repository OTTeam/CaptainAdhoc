#include "utils.h"
#include "ServeurTCP.h"
#include <QMessageBox>

TCPServer::TCPServer(QObject *parent) :
    QObject(parent)
{

    if (!StartServer())
    {
        QMessageBox::information(0,"Serveur TCP","Serveur non démarré",QMessageBox::Ok);
    }
}



bool TCPServer::ServerStarted()
{
    return _serverStarted;
}


bool TCPServer::StartServer()
{
    // Gestion du serveur
    _serveur = new QTcpServer(this);
    if (!_serveur->listen(QHostAddress::Any, PORT_SERVEUR)) // Démarrage du serveur sur toutes les IP disponibles et sur le port 50585
    {
        _serverStarted = false;
    }
    else
    {
        _serverStarted = true;
        connect(_serveur, SIGNAL(newConnection()), this, SLOT(nouvelleConnexion()));
    }
    return _serverStarted;
}


void TCPServer::nouvelleConnexion()
{
    QTcpSocket *nouveauClient = _serveur->nextPendingConnection();
    emit AjouterClient(nouveauClient);
}
