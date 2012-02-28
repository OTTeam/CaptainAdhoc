#ifndef WIFINETWORK_H
#define WIFINETWORK_H

#include <QObject>
#include <Windows.h>
#include <adhoc.h>

class WifiNetwork : public QObject
{
    Q_OBJECT

public:

    WifiNetwork (IDot11AdHocNetwork *);
    ~WifiNetwork();

    bool Connect(QString password);

    QString GetSSID();

private:
    IDot11AdHocNetwork * _network;

};

#endif // WIFINETWORK_H
