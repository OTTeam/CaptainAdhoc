#include "WifiNetwork.h"

#include <QDebug>
#include <windows.h>
#include <adhoc.h>
#include <iostream>
#include <OCIdl.h>

WifiNetwork::WifiNetwork(IDot11AdHocNetwork * network)
{
#ifdef TRACE
    qDebug() << "[CONS] WifiNetwork";
#endif
    _network = network;
    _registered = false;
}

WifiNetwork::~WifiNetwork()
{
#ifdef TRACE
    qDebug() << "[DEST] WifiNetwork";
#endif
    Disconnect();
    UnregisterNetworkNotifications();
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
    WCHAR pwd[40];
    password.toWCharArray(pwd);
    pwd[password.length()] = NULL;

    qDebug() << "Connecting... ";
    HRESULT ans = _network->Connect(pwd, 0x54, false, false);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    return SUCCEEDED(ans);
}


bool WifiNetwork::Disconnect()
{
    HRESULT ans;
    ans = _network->Disconnect();
    return SUCCEEDED(ans);
}

void WifiNetwork::RegisterNetworkNotifications()
{
    if (!_registered)
    {
        HRESULT ans;
        IConnectionPointContainer  * pConnectionPointContainer;
        IConnectionPoint * pConnectionPoint;

        _networkSink = new NetworkNotificationSink();
        connect( _networkSink, SIGNAL(ConnectionStatusChanged(int)), this, SIGNAL(ConnectionStatusChanged(int)) );
        connect( _networkSink, SIGNAL(ConnectionFail(int)), this, SIGNAL(ConnectionStatusChanged(int)) );

        qDebug() << "Casting Network in ConnectionPointContainer... ";
        ans = _network->QueryInterface(IID_IConnectionPointContainer, (void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink, &pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Registering for network notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) _networkSink, &_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        if (SUCCEEDED(ans))
        {
            _registered = true;
        }
    }
}


void WifiNetwork::UnregisterNetworkNotifications()
{
    if (_registered)
    {
        HRESULT ans;
        IConnectionPointContainer * pConnectionPointContainer;
        IConnectionPoint * pConnectionPoint;

        qDebug() << "Casting Network in ConnectionPointContainer... ";

        ans = _network->QueryInterface(IID_IConnectionPointContainer, (void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink, &pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Unregistering for network notifications... ";
        ans = pConnectionPoint->Unadvise(_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        delete _networkSink;

        if (SUCCEEDED(ans))
        {
            _registered = false;
        }
    }
}
