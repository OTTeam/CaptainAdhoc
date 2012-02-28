#include "WifiManager.h"


#include <windows.h>
#include <adhoc.h>
#include <iostream>
#include <OCIdl.h>

using namespace std;

WifiManager::WifiManager(){

}

WifiManager::~WifiManager()
{
    DisconnectWifi();
}

void WifiManager::ConnectWifi()
{
    IDot11AdHocManager * AdHocManager = NULL;
    IEnumDot11AdHocNetworks * networks = NULL;
    HRESULT ans;
    ULONG cnt;
    IDot11AdHocNetwork * network [10];
    IConnectionPointContainer  * pConnectionPointContainer;
    IConnectionPoint * pConnectionPoint;
    LPWSTR ssid;
    cout << "Initialising COM Lib... ";
    ans = CoInitialize(NULL);
    cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

    cout << "Creating AdHocManager... ";
    ans = CoCreateInstance(CLSID_Dot11AdHocManager,NULL,CLSCTX_INPROC_SERVER,IID_IDot11AdHocManager,(void**) &AdHocManager);
    cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

    cout << "Casting AdHocManager in ConnectionPointContainer... ";
    ans = AdHocManager->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
    cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

    cout << "Retrieving connection point for AdHocManagerNotifications... ";
    ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocManagerNotificationSink,&pConnectionPoint);
    cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;



    DWORD sinkCookie;

    cout << "Registering for notifications... ";
    ans = pConnectionPoint->Advise((IUnknown*) &mSink, &sinkCookie);
    cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

    cout << "Getting Network list... ";
    ans = AdHocManager->GetIEnumDot11AdHocNetworks(NULL, &networks);

    cout << "OK" << endl  << "Extracting Networks... ";
    ans = networks->Next(10,network,&cnt);

    printf("Got %d networks\n",cnt);

    bool found = false;
    IDot11AdHocNetwork * myNet;

    for (ULONG i = 0; i < cnt; i++)
    {
        network[i]->GetSSID(&ssid);
        wstring str(ssid);
        wcout << "\tssid = " << str << endl;

        if (str == ADHOC_SSID)
        {
            found=true;
            myNet = network[i];
        }
    }

     if (found)
    {
        cout << "Casting NetWork in ConnectionPointContainer... ";
        ans = myNet->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
        cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


        cout << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink,&pConnectionPoint);
        cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

        cout << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &nSink,&sinkCookie);
        cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


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
    else
    {
        printf("Creating the network... ");

        SecuritySettings securitySettings;
        ans = AdHocManager->CreateNetwork(ADHOC_SSID, ADHOC_PWD, 0x54, NULL, &securitySettings, NULL, &myNet);
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

        cout << "Casting NetWork in ConnectionPointContainer... ";
        ans = myNet->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
        cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;


        cout << "Retrieving connection point for NetworkNotifications... ";
        ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocNetworkNotificationSink,&pConnectionPoint);
        cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

        cout << "Registering for notifications... ";
        ans = pConnectionPoint->Advise((IUnknown*) &nSink,&sinkCookie);
        cout << ((SUCCEEDED(ans)) ? "OK" : "KO") << endl;

        cout << "Committing the network... ";
        ans = AdHocManager->CommitCreatedNetwork(myNet,false,false);
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

}
