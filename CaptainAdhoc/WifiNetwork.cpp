#include "WifiNetwork.h"
#include <QDebug>

WifiNetwork::WifiNetwork(IDot11AdHocNetwork * network)
{
    _network = network;
}

WifiNetwork::~WifiNetwork()
{

}

QString WifiNetwork::GetSSID()
{
    LPWSTR pSSID;
    _network->GetSSID(&pSSID);
    QString ssid = QString::fromWCharArray(pSSID);
    return ssid;
}

bool WifiNetwork::Connect(QString password)
{
    HRESULT ans;

    LPWSTR pwd = new WCHAR(password.length()+1);
    password.toWCharArray(pwd);
    pwd[password.length()]=NULL;

    ans = _network->Connect(pwd,0x54,false,false);

    return SUCCEEDED(ans);
}
