#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include "utils.h"
#include "ManagerNotificationSink.h"
#include "NetworkNotificationSink.h"
#include "SecuritySettings.h"

// SSID du reseau :
#define ADHOC_SSID "CAPTAIN_ADHOC"
#define W_ADHOC_SSID L"CAPTAIN_ADHOC"

// Mot de passe du reseau :
#define ADHOC_PWD L"archibald1941"

class WifiManager : public QObject
{
    Q_OBJECT
public:
    WifiManager();
    ~WifiManager();

    void RegisterNotifications();
    void UnregisterNotifications();

public slots:
    bool ConnectWifi();     //Connexion au r�seau WiFi AdHoc - Retourne vrai si connect�
    bool DisconnectWifi();  //D�connexion au r�seau WiFi AdHoc - Retourne vrai si deconnect�

signals:
    void ConnectionStatusChanged(CONNECTION_STATUS);

private:
    IDot11AdHocManager * _adHocManager;
    IDot11AdHocNetwork * _network;

    ManagerNotificationSink _managerSink;
    NetworkNotificationSink _networkSink;

    DWORD _sinkCookie;

    bool _connected;
    bool _registered;
};

#endif // WIFIMANAGER_H
