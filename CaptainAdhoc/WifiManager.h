#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include "utils.h"
#include "ManagerNotificationSink.h"
#include "NetworkNotificationSink.h"
#include "SecuritySettings.h"

// SSID du reseau :
#define ADHOC_SSID L"CAPTAIN_ADHOC"
// Mot de passe du reseau :
#define ADHOC_PWD L"archibald1941"

class WifiManager : public QObject
{
    Q_OBJECT
public:
    WifiManager();
    ~WifiManager();

public slots:
    void ConnectWifi();
    void DisconnectWifi();

signals:
    void ConnectionStatusChanged(CONNECTION_STATUS);

private:
    ManagerNotificationSink mSink;
    NetworkNotificationSink nSink;

    IDot11AdHocNetwork * myNet;
};

#endif // WIFIMANAGER_H
