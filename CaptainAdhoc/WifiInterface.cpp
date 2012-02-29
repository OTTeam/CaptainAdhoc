#include "WifiInterface.h"
#include <QDebug>
#include <Windows.h>
#include <adhoc.h>
#include "utils.h"

WifiInterface::WifiInterface(IDot11AdHocInterface * wifiCard)
{
#ifdef TRACE
    qDebug() << "[CONS] WifiInterface";
#endif
    _wifiCard = wifiCard;
}

WifiInterface::~WifiInterface()
{
#ifdef TRACE
    qDebug() << "[DEST] WifiInterface";
#endif
}

QString WifiInterface::GetName()
{
    LPWSTR name;
    _wifiCard->GetFriendlyName(&name);
    QString qName = QString::fromWCharArray(name);
    return qName;
}

bool WifiInterface::IsRadioOn()
{
    BOOLEAN ans;
    _wifiCard->IsRadioOn(&ans);
    return ans;
}

int WifiInterface::GetStatus()
{
    DOT11_ADHOC_NETWORK_CONNECTION_STATUS status;
    _wifiCard->GetStatus(&status);
    switch(status)
    {
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_DISCONNECTED:
        return DISCONNECTED;
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_CONNECTED:
        return CONNECTED;
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_FORMED:
        return FORMED;
    default:
        return -1;
    }

}

bool WifiInterface::DisconnectWifi()
{
    IDot11AdHocNetwork * net;
    HRESULT ans = _wifiCard->GetActiveNetwork(&net);

    if (FAILED(ans))
    {
        qDebug() << "Can't get an active network for the interface" << GetName();
        return false;
    }

    if (!net)
    {
        qDebug() << "Connected to a non adhoc network, can't disconnect...";
        return false;
    }

    ans = net->Disconnect();
    if (FAILED(ans))
    {
        qDebug() << "Disconnect call on the network failed";
        return false;
    }

    qDebug() << "Successfully disconnected";
    return true;

}
