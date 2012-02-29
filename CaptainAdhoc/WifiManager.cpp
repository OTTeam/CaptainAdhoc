#include "WifiManager.h"

#include <QDebug>
#include <windows.h>
#include <adhoc.h>
#include <iostream>
#include <OCIdl.h>

using namespace std;


WifiManager::WifiManager()
{
#ifdef TRACE
    qDebug() << "[CONS] WifiManager";
#endif

    _connected = false;
    _registered = false;

    qDebug() << endl << "Initialising COM Lib... ";
    HRESULT ans = CoInitialize(NULL);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Creating AdHocManager... ";
    ans = CoCreateInstance(CLSID_Dot11AdHocManager, NULL, CLSCTX_INPROC_SERVER, IID_IDot11AdHocManager, (LPVOID*) &_adHocManager);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    connect( &_networkSink, SIGNAL(ConnectionStatusChanged(int)), this, SLOT(onConnectionStatusChanged(int)) );
}

WifiManager::~WifiManager()
{
#ifdef TRACE
    qDebug() << "[DEST] WifiManager";
#endif
    DisconnectWifi();

    UnregisterNotifications();

    qDebug() << "Fermeture de la Lib COM...";
    CoUninitialize();
    qDebug() << "Done";
}


void WifiManager::RegisterNotifications()
{
    IConnectionPointContainer  * pCPC;
    IConnectionPoint * pCP;

    qDebug() << "Casting AdHocManager in ConnectionPointContainer... ";
    HRESULT ans = _adHocManager->QueryInterface(IID_IConnectionPointContainer,(void**) &pCPC);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Retrieving connection point for AdHocManagerNotifications... ";
    ans = pCPC->FindConnectionPoint(IID_IDot11AdHocManagerNotificationSink,&pCP);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Registering for notifications... ";
    ans = pCP->Advise((IUnknown*) &_managerSink, &_sinkCookie);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    if (SUCCEEDED(ans))
    {
        _registered = true;
    }
}

void WifiManager::UnregisterNotifications()
{
    if (_registered)
    {
        IConnectionPointContainer  * pCPC;
        IConnectionPoint * pCP;

        qDebug() << "Casting AdHocManager in ConnectionPointContainer... ";
        HRESULT ans = _adHocManager->QueryInterface(IID_IConnectionPointContainer,(void**) &pCPC);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Retrieving connection point for AdHocManagerNotifications... ";
        ans = pCPC->FindConnectionPoint(IID_IDot11AdHocManagerNotificationSink,&pCP);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Unregistering for notifications... ";
        ans = pCP->Unadvise(_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        if (SUCCEEDED(ans))
        {
            _registered = false;
        }
    }
}

QList<WifiInterface*> * WifiManager::GetInterfaces()
{
    QList<WifiInterface*> * intList = new QList<WifiInterface*>();
    IEnumDot11AdHocInterfaces * interfaces;
    IDot11AdHocInterface * intface;

    ULONG cnt;

    qDebug() << "Querying Interfaces list ...";
    HRESULT ans = _adHocManager->GetIEnumDot11AdHocInterfaces(&interfaces);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Retrieving first interface...";
    interfaces->Next(1,&intface,&cnt);

    while (SUCCEEDED(ans))
    {
        WifiInterface * wifi = new WifiInterface(intface);
        intList->append(wifi);
        ans = interfaces->Next(1,&intface,&cnt);
    }
    return intList;

}

void WifiManager::DeleteInterfaceList(QList<WifiInterface*> * list)
{
    foreach(WifiInterface* wifi,*list)
    {
        delete wifi;
    }
    delete list;
}

QList<WifiNetwork*> * WifiManager::GetNetworks()
{
    QList<WifiNetwork*> * list = new QList<WifiNetwork*>();

    IEnumDot11AdHocNetworks * networks = NULL;
    IDot11AdHocNetwork * network;

    HRESULT ans;
    ULONG cnt;

    qDebug() << "Getting Network list... ";
    ans = _adHocManager->GetIEnumDot11AdHocNetworks(NULL, &networks);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Extracting Networks... ";
    ans = networks->Next(1,&network,&cnt);
    while (SUCCEEDED(ans))
    {
        WifiNetwork * wifi = new WifiNetwork(network);
        list->append(wifi);
        ans = networks->Next(1,&network,&cnt);
    }

    return list;

}

void WifiManager::DeleteNetworkList(QList<WifiNetwork *> * list)
{
    foreach(WifiNetwork * net,*list)
    {
        delete net;
    }
    delete list;
}

void WifiManager::ConnectWifi()
{
//    if( !_connected )
//    {
//        IConnectionPointContainer  * pConnectionPointContainer;
//        IConnectionPoint * pConnectionPoint;
//        LPWSTR ssid;

//        qDebug() << "Got " << cnt << " networks";

//        bool found = false;

//        for (ULONG i = 0; i < cnt; i++)
//        {
//            network[i]->GetSSID(&ssid);
//            QString qSSID = QString::fromWCharArray(ssid);
//            qDebug() << "Network" << i+1 << ":" << qSSID;

//            if (qSSID == ADHOC_SSID)
//            {
//                found=true;
//                _network = network[i];
//            }
//        }

//        if (found) //Si le reseau existe deja
//        {
//            qDebug() << "Casting NetWork in ConnectionPointContainer... ";
//            ans = _network->QueryInterface(IID_IConnectionPointContainer, (void**) &pConnectionPointContainer);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");


//            qDebug() << "Retrieving connection point for NetworkNotifications... ";
//            ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink, &pConnectionPoint);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

//            qDebug() << "Registering for notifications... ";
//            ans = pConnectionPoint->Advise((IUnknown*) &_networkSink, &_sinkCookie);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");


//            qDebug() << "Connecting... ";
//            ans = _network->Connect(ADHOC_PWD, 0x54, false, false);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");
//        }
//        else //Si le reseau n'existe pas, il faut le créer
//        {
//            qDebug() << "Creating the network... ";

//            SecuritySettings securitySettings;

//            ans = _adHocManager->CreateNetwork(W_ADHOC_SSID, ADHOC_PWD, 0x54, NULL, &securitySettings, NULL, &_network);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

//            qDebug() << "Casting NetWork in ConnectionPointContainer... ";
//            ans = _network->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


//            qDebug() << "Retrieving connection point for NetworkNotifications... ";
//            ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink, &pConnectionPoint);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

//            qDebug() << "Registering for notifications... ";
//            ans = pConnectionPoint->Advise((IUnknown*) &_networkSink, &_sinkCookie);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

//            qDebug() << "Committing the network... ";
//            ans = _adHocManager->CommitCreatedNetwork(_network, false, false);
//            qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");
//        }
//    }
}

void WifiManager::DisconnectWifi()
{
    if(_connected)
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

        qDebug() << "Disconnecting... ";
        ans = _network->Disconnect();
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");
    }
}

void WifiManager::onConnectionStatusChanged(int status)
{
    switch (status)
    {
    case FORMED:
        qDebug() << "Notification received : network formed";
        _connected = true;
        break;
    case CONNECTED:
        qDebug() << "Notification received : connected to network";
        _connected = true;
        break;
    case DISCONNECTED:
        qDebug() << "Notification received : disconnected from network";
        _connected = false;
        break;
    }

}
