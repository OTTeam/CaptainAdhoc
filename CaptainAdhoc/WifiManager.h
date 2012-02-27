#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>
#include "utils.h"

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
};

#endif // WIFIMANAGER_H
