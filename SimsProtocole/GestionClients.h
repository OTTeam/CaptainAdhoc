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
    void ClientUploadUpdate(Client *,int);
    void ClientDownloadUpdate(Client *,int);

    void ClientUploadSpeedUpdate(Client *,int);
    void ClientDownloadSpeedUpdate(Client *,int);

    void ClientNumberChanged(int);

private slots:
    void clientBytesAvailable();
    void clientBytesWritten(qint64);

    void clientReceived(int);
    void clientSent(int);

    void newConnectionRequest(QHostAddress broadcasterAddress,QList<RoutesTableElt> routes);
    void newConnectionDone(QTcpSocket *);

    void clientDisconnect();
    void clientConnectionFailed();
    void clientConnected();
    void NewClientConfig(Client *client);

    void uploadSpeedUpdate(int);
    void downloadSpeedUpdate(int);

    void broadCastTrigger();
private:
    Client *findClientByDest(QHostAddress);
    SocketsHandlers *findSocketHandler(QTcpSocket *);

private:
    QList<Client*> _clients;
    QList<SocketsHandlers *> _socketsHandlers;
    TCPServer *_listeningServer;
    ClientDiscovery *_clientDiscoveryModule;
    QList<LocalFiles> _localFiles;
    QList<RemoteFiles> _remoteFiles;

    QTimer *_timerBroadcast;
};


#endif // GESTIONCLIENTS_H
