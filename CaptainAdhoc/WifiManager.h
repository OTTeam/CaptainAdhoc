#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include "ManagerNotificationSink.h"
#include "NetworkNotificationSink.h"
#include "SecuritySettings.h"
#include "WifiInterface.h"
#include "WifiNetwork.h"

class WifiManager : public QObject
{
    Q_OBJECT
public:

    // Constructeur et destructeur
    WifiManager();
    virtual ~WifiManager();

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
