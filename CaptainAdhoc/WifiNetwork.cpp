#include "WifiNetwork.h"

#include <QDebug>
#include <windows.h>
#include <adhoc.h>
#include <iostream>
#include <OCIdl.h>

WifiNetwork::WifiNetwork(IDot11AdHocNetwork * network)
{
    _network = network;
    _registered = false;

    connect( &_networkSink, SIGNAL(ConnectionStatusChanged(int)), this, SIGNAL(ConnectionStatusChanged(int)) );
    connect( &_networkSink, SIGNAL(ConnectionFail(int)), this, SIGNAL(ConnectionStatusChanged(int)) );
}

WifiNetwork::~WifiNetwork()
{
    delete _network;
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
    LPWSTR pwd = new WCHAR(password.length()+1);
    password.toWCharArray(pwd);
    pwd[password.length()] = NULL;

    qDebug() << "Connecting... ";
    HRESULT ans = _network->Connect(pwd, 0x54, false, false);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    delete pwd;

    return SUCCEEDED(ans);
}



bool WifiNetwork::Disconnect()
{
    HRESULT ans;
    ans = _network->Disconnect();
    return SUCCEEDED(ans);
}

void WifiNetwork::RegisterNotifications()
{
    if (!_registered)
    {
        HRESULT ans;
        IConnectionPointContainer  * pConnectionPointContainer;
        IConnectionPoint * pConnectionPoint;

        qDebug() << "Casting NetWork in ConnectionPointContainer... ";
        ans = _network->QueryInterface(IID_IConnectionPointContainer, (void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink, &pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &_networkSink, &_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        if (SUCCEEDED(ans))
        {
            _registered = true;
        }
    }
}

void WifiNetwork::UnregisterNotifications()
{
    if (_registered)
    {
        HRESULT ans;
        IConnectionPointContainer  * pConnectionPointContainer;
        IConnectionPoint * pConnectionPoint;

        qDebug() << "Casting NetWork in ConnectionPointContainer... ";

        ans = _network->QueryInterface(IID_IConnectionPointContainer, (void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink, &pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Unregistering for notifications... ";
        ans = pConnectionPoint->Unadvise(_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        if (SUCCEEDED(ans))
        {
            _registered = false;
        }
    }
}


//void WifiNetwork::onConnectionStatusChanged(int status)
//{
//    switch (status)
//    {
//    case FORMED:
//        qDebug() << "Notification received : network formed";
//        break;
//    case CONNECTED:
//        qDebug() << "Notification received : connected to network";
//        break;
//    case DISCONNECTED:
//        qDebug() << "Notification received : disconnected from network";
//        break;
//    }
//}

//void WifiNetwork::onConnectionFail(int reason)
//{
//    switch(reason)
//    {
//    case DOT11_ADHOC_CONNECT_FAIL_DOMAIN_MISMATCH:
//        qDebug() << "Notification received : connection fail (domain mismatch)";
//        break;
//    case DOT11_ADHOC_CONNECT_FAIL_PASSPHRASE_MISMATCH:
//        qDebug() << "Notification received : connection fail (pwd mismatch)";
//        break;
//    case DOT11_ADHOC_CONNECT_FAIL_OTHER:
//        qDebug() << "Notification received : connection fail";
//        break;
//    }
//}
