#ifndef WIFINETWORK_H
#define WIFINETWORK_H

#include <QObject>
#include <Windows.h>
#include <adhoc.h>
#include "NetworkNotificationSink.h"

class WifiNetwork : public QObject
{
    Q_OBJECT

public:
    WifiNetwork (IDot11AdHocNetwork *);
    virtual ~WifiNetwork();

    bool Connect(QString password);
    bool Disconnect();

    void RegisterNetworkNotifications();
    void UnregisterNetworkNotifications();

    QString GetSSID();

private:
    IDot11AdHocNetwork * _network;
    NetworkNotificationSink * _networkSink;
    DWORD _sinkCookie;
    bool _registered;

signals:
    void ConnectionStatusChanged(int);
    void ConnectionFail(int);

};

#endif // WIFINETWORK_H
