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
    //void TransfertUpdate(int);
    void ClientUploadUpdate(Client *,int);
    void ClientDownloadUpdate(Client *,int);

    //void NetworkSpeedUpdate(int);
    void ClientUploadSpeedUpdate(Client *,int);
    void ClientDownloadSpeedUpdate(Client *,int);

    void ClientNumberChanged(int);

private slots:
    void clientReceived(int);
    void clientSent(int);

    void newConnectionRequest(QHostAddress);
    void newConnectionDone(QTcpSocket *);

    void clientDisconnect();
    void clientConnected(Client *);

    void uploadSpeedUpdate(int);
    void downloadSpeedUpdate(int);

private:
    QList<Client*> clients;
    TCPServer *listeningServer;
    ClientDiscovery *clientDiscoveryModule;
    QList<LocalFiles> localFiles;
    QList<RemoteFiles> remoteFiles;
};


#endif // GESTIONCLIENTS_H
