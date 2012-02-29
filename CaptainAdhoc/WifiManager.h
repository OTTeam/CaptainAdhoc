#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include "utils.h"
#include "ManagerNotificationSink.h"
#include "NetworkNotificationSink.h"
#include "SecuritySettings.h"
#include "WifiInterface.h"
#include "WifiNetwork.h"

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

    QList<WifiInterface*> * GetInterfaces();
    void DeleteInterfaceList(QList<WifiInterface*>*);

    QList<WifiNetwork*> * GetNetworks();
    void DeleteNetworkList(QList<WifiNetwork*>*);

public slots:
    void ConnectWifi();     //Connexion au réseau WiFi AdHoc
    void DisconnectWifi();  //Déconnexion au réseau WiFi AdHoc

private:
    IDot11AdHocManager * _adHocManager;
    IDot11AdHocNetwork * _network;

    ManagerNotificationSink _managerSink;
    DWORD _sinkCookie;

    bool _connected;
    bool _registered;
};

#endif // WIFIMANAGER_H
