#ifndef CLIENTDISCOVERY_H
#define CLIENTDISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

#include "Client.h"

class ClientDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit ClientDiscovery(QObject *parent = 0);
    
signals:
    void DatagramReceived(QHostAddress,QList<RoutesTableElt>);


public slots:
    void sendNewDatagram(QList<Client *> routesList);

private slots:
    void newDatagramAvailable();

private:
    QUdpSocket *_socket;
    
};

#endif // CLIENTDISCOVERY_H
