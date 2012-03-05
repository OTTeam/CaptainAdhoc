#ifndef WIFIINTERFACE_H
#define WIFIINTERFACE_H

#include <QObject>
#include <Windows.h>
#include <adhoc.h>

class WifiInterface : public QObject
{
    Q_OBJECT
public:

    WifiInterface (IDot11AdHocInterface *wifiCard);
    virtual ~WifiInterface();

    QString GetName();
    bool IsRadioOn();
    int GetStatus();

    bool DisconnectWifi();

private:

    IDot11AdHocInterface * _wifiCard;


};

#endif // WIFIINTERFACE_H
