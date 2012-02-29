#include "WifiInterface.h"
#include <QDebug>
#include <Windows.h>
#include <adhoc.h>
#include "utils.h"

WifiInterface::WifiInterface(IDot11AdHocInterface * wifiCard)
{
    _wifiCard = wifiCard;
}

WifiInterface::~WifiInterface()
{

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
