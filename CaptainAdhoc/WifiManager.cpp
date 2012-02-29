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

    _registered = false;

    qDebug() << endl << "Initialising COM Lib... ";
    HRESULT ans = CoInitialize(NULL);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Creating AdHocManager... ";
    ans = CoCreateInstance(CLSID_Dot11AdHocManager, NULL, CLSCTX_INPROC_SERVER, IID_IDot11AdHocManager, (LPVOID*) &_adHocManager);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

}

WifiManager::~WifiManager()
{
#ifdef TRACE
    qDebug() << "[DEST] WifiManager";
#endif

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

WifiNetwork * WifiManager::CreateWifi(QString SSID, QString password)
{
    WifiNetwork * net;
    IDot11AdHocNetwork * dot11Net;
    SecuritySettings securitySettings;
    HRESULT ans;

    LPWSTR ssid = new WCHAR[SSID.length()+1];
    SSID.toWCharArray(ssid);
    ssid[SSID.length()]=NULL;

    LPWSTR pwd = new WCHAR[password.length()+1];
    password.toWCharArray(pwd);
    pwd[password.length()]=NULL;

    qDebug() << "Creation du network...";
    ans = _adHocManager->CreateNetwork(ssid, pwd, 0x54, NULL, &securitySettings, NULL, &dot11Net);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Committing the network... ";
    ans = _adHocManager->CommitCreatedNetwork(dot11Net, false, false);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    net = new WifiNetwork(dot11Net);

    return net;

}



