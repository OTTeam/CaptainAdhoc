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
    IConnectionPointContainer  * pConnectionPointContainer;
    IConnectionPoint * pConnectionPoint;

    qDebug() << "Casting AdHocManager in ConnectionPointContainer... ";
    HRESULT ans = _adHocManager->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Retrieving connection point for AdHocManagerNotifications... ";
    ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocManagerNotificationSink,&pConnectionPoint);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Registering for notifications... ";
    ans = pConnectionPoint->Advise((IUnknown*) &_sink, &_sinkCookie);
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

    IConnectionPointContainer  * pConnectionPointContainer;
    IConnectionPoint * pConnectionPoint;

    qDebug() << "Casting AdHocManager in ConnectionPointContainer... ";
    HRESULT ans = _adHocManager->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Retrieving connection point for AdHocManagerNotifications... ";
    ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocManagerNotificationSink,&pConnectionPoint);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    qDebug() << "Unregistering for notifications... ";
    ans = pConnectionPoint->Unadvise(_sinkCookie);
    qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO");

    _registered = false;

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
        wstring str(ssid);
        qDebug() << "Network" << i+1 << ":" << *ssid;

        if (str == ADHOC_SSID)
        {
            found=true;
            myNet = network[i];
        }
    }

    if (found) //Si le reseau existe deja
    {
        qDebug() << "Casting NetWork in ConnectionPointContainer... ";
        ans = myNet->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink,&pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

        qDebug() << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &nSink,&_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


        printf("Connecting... ");
        ans = myNet->Connect(ADHOC_PWD, 0x54, false, false);
        switch (ans)
        {
        case S_OK:
            printf("OK");
            break;
        case REGDB_E_CLASSNOTREG:
            printf("REGDB");
            break;
        case CLASS_E_NOAGGREGATION:
            printf("CLASS");
            break;
        case E_NOINTERFACE:
            printf("NOINTERFACE");
            break;
        case E_POINTER:
            printf("POINTER");
            break;
        default :
            printf("%d",ans);
        }

        printf("\n");
    }
    else //Si le reseau n'existe pas, il faut le créer
    {
        printf("Creating the network... ");

        SecuritySettings securitySettings;
        ans = _adHocManager->CreateNetwork(ADHOC_SSID, ADHOC_PWD, 0x54, NULL, &securitySettings, NULL, &myNet);
        switch (ans)
        {
        case S_OK:
            printf("OK");
            break;
        case REGDB_E_CLASSNOTREG:
            printf("REGDB");
            break;
        case CLASS_E_NOAGGREGATION:
            printf("CLASS");
            break;
        case E_NOINTERFACE:
            printf("NOINTERFACE");
            break;
        case E_POINTER:
            printf("POINTER");
            break;
        default :
            printf("%d",ans);
        }

        printf("\n");

        qDebug() << "Casting NetWork in ConnectionPointContainer... ";
        ans = myNet->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


        qDebug() << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink,&pConnectionPoint);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

        qDebug() << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &nSink,&_sinkCookie);
        qDebug() << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

        qDebug() << "Committing the network... ";
        ans = _adHocManager->CommitCreatedNetwork(myNet,false,false);
        switch (ans)
        {
        case S_OK:
            printf("OK");
            break;
        case REGDB_E_CLASSNOTREG:
            printf("REGDB");
            break;
        case CLASS_E_NOAGGREGATION:
            printf("CLASS");
            break;
        case E_NOINTERFACE:
            printf("NOINTERFACE");
            break;
        case E_POINTER:
            printf("POINTER");
            break;
        default :
            printf("%d",ans);
        }

        printf("\n");
    }
}

void WifiManager::DisconnectWifi()
{

    myNet->Disconnect();
}
