#ifndef GESTIONCLIENTS_H
#define GESTIONCLIENTS_H

#include <QObject>
#include <QTcpSocket>
#include <QList>

#include "Client.h"
#include "ServeurTCP.h"
#include "ClientDiscovery.h"


class GestionClients : public QObject
{
    Q_OBJECT
public:
    explicit GestionClients(QObject *parent = 0);
    void sendToAll();

    
signals:
    void TransfertUpdate(int);
    void NetworkSpeedUpdate(int);
    void ClientNumberChanged(int);

private slots:
    void clientReceived(int);
    void newConnectionRequest(QHostAddress);
    void newConnectionDone(QTcpSocket *);
    void clientDisconnect();
    void clientConnected(Client *);

    void dlSpeedUpdate(int);

private:
    QList<Client*> clients;
    TCPServer *serveurEcoute;
    ClientDiscovery *clientDiscoveryModule;
    QList<LocalFiles> fichiersDisponibles;
    QList<RemoteFiles> fichiers;
};


#endif // GESTIONCLIENTS_H
