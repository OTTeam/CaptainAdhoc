#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <QObject>

// SSID du reseau :
#define ADHOC_SSID L"CAPTAIN_ADHOC"
// Mot de passe du reseau (statique pour le moment, mais c'est moyen secure...) :
#define ADHOC_PWD L"1491_archibald_1941"

class WifiManager : public QObject
{
    Q_OBJECT
public:
    WifiManager();
    ~WifiManager();

public slots:
    void ConnectWifi();
    void DisconnectWifi();
};

#endif // WIFIMANAGER_H
