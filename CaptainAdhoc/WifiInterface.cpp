#include "WifiInterface.h"
#include <QDebug>
#include <Windows.h>
#include <adhoc.h>

WifiInterface::WifiInterface(IDot11AdHocInterface * wifiCard)
{
    _wifiCard = wifiCard;
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
