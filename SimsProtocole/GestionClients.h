#ifndef GESTIONCLIENTS_H
#define GESTIONCLIENTS_H

#include <QObject>
#include <QTcpSocket>
#include <QList>

#include "Client.h"
#include "ServeurTCP.h"


class GestionClients : public QObject
{
    Q_OBJECT
public:
    explicit GestionClients(QObject *parent = 0);
    void sendToAll();

    
signals:
    void TransfertUpdate(int);
private slots:
    void clientReceived(int);
    void newConnectionRequest(QString);
    void newConnectionDone(QTcpSocket *);
    void clientDisconnect();

private:
    QList<Client*> clients;
    ServeurTCP *serveurEcoute;
    QList<LocalFiles> fichiersDisponibles;
    QList<RemoteFiles> fichiers;
};


#endif // GESTIONCLIENTS_H
