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

private:
    WifiManager * _manager;
    WifiNetwork * _network;
};

#endif // WIFICONNECTION_H
