#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <QObject>
#include "WifiManager.h"

class WifiConnection : public QObject
{
    Q_OBJECT

public:
    WifiConnection();
    virtual ~WifiConnection();

public slots:
    void Connect();
    void Disconnect();

signals:
    void ConnectionStatusChanged(int);
    void ConnectionFail(int);

private slots:
    void onConnectionStatusChanged(int);
    void onConnectionFail(int);

private:
    WifiManager * _manager;
    WifiNetwork * _network;
};

#endif // WIFICONNECTION_H
