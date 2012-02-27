#ifndef CLIENTDISCOVERY_H
#define CLIENTDISCOVERY_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>

class ClientDiscovery : public QObject
{
    Q_OBJECT
public:
    explicit ClientDiscovery(QObject *parent = 0);
    
signals:
    void DatagramReceived(QHostAddress);

private slots:
    void newDatagramAvailable();
    void sendNewDatagram();
private:
    QUdpSocket *socket;
    QTimer *timer;
    
};

#endif // CLIENTDISCOVERY_H
