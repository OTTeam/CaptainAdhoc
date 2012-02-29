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
#define ADHOC_PWD "archibald1941"
#define W_ADHOC_PWD L"archibald1941"

class WifiManager : public QObject
{
    Q_OBJECT
public:

    // Constructeur et destructeur
    WifiManager();
    ~WifiManager();

    // Gestion des notifications
    void RegisterNotifications();
    void UnregisterNotifications();

    // Gestion des Interfaces (cartes wifi)
    QList<WifiInterface*> * GetInterfaces();
    void DeleteInterfaceList(QList<WifiInterface*>*);

    // Gestion des réseaux
    QList<WifiNetwork*> * GetNetworks();
    void DeleteNetworkList(QList<WifiNetwork*>*);

    // Création d'un réseau wifi adhoc
    WifiNetwork * CreateWifi(QString ssid, QString pwd);


private:

    // L'objet windows de gestion des réseaux wifi
    IDot11AdHocManager * _adHocManager;

    // Le point de réception des notificaqtions wifi windows
    ManagerNotificationSink _managerSink;

    // permet de désenregistrer les notifications
    DWORD _sinkCookie;

    //flag pour savoir si les notifs sont enregistrées ou non
    bool _registered;
};

#endif // WIFIMANAGER_H
