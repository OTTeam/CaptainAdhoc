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
    ans = CoCreateInstance(CLSID_Dot11AdHocManager,NULL,CLSCTX_INPROC_SERVER,IID_IDot11AdHocManager,(LPVOID*) &_adHocManager);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

}

WifiManager::~WifiManager()
{
#ifdef TRACE
    cout << "[DEST] WifiManager" << endl;
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
    ans = pCP->Advise((IUnknown*) &_sink, &_sinkCookie);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    if (SUCCEEDED(ans))
    {
        _registered = true;
    }

}

void WifiManager::UnregisterNotifications()
{
    if (!_registered)
        return;

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

    _registered = false;

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

void WifiManager::ConnectWifi()
{

    IEnumDot11AdHocNetworks * networks = NULL;
    HRESULT ans;
    ULONG cnt;
    IDot11AdHocNetwork * network [10];
    IConnectionPointContainer  * pConnectionPointContainer;
    IConnectionPoint * pConnectionPoint;
    LPWSTR ssid;







    qDebug() << "Getting Network list... ";
    ans = _adHocManager->GetIEnumDot11AdHocNetworks(NULL, &networks);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Extracting Networks... ";
    ans = networks->Next(10,network,&cnt);

    qDebug() << "Got"<< cnt << "networks";

    bool found = false;

    for (ULONG i = 0; i < cnt; i++)
    {
        network[i]->GetSSID(&ssid);
        QString qSSID = QString::fromWCharArray(ssid);
        qDebug() << "Network" << i+1 << ":" << qSSID;

        if (qSSID == ADHOC_SSID)
        {
            found=true;
            myNet = network[i];
        }
    }

    if (found) //Si le reseau existe deja
    {
        qDebug() << "Casting NetWork in ConnectionPointContainer... ";
        ans = myNet->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");


        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink,&pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &nSink,&_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");


        qDebug() << "Connecting... ";
        ans = myNet->Connect(ADHOC_PWD, 0x54, false, false);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");
    }
    else //Si le reseau n'existe pas, il faut le créer
    {
        qDebug() << "Creating the network... ";

        SecuritySettings securitySettings;

        ans = _adHocManager->CreateNetwork(W_ADHOC_SSID, ADHOC_PWD, 0x54, NULL, &securitySettings, NULL, &myNet);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Casting NetWork in ConnectionPointContainer... ";
        ans = myNet->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");


        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink,&pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &nSink,&_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

        qDebug() << "Committing the network... ";
        ans = _adHocManager->CommitCreatedNetwork(myNet,false,false);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");
    }
}

void WifiManager::DisconnectWifi()
{

    myNet->Disconnect();
}
