#include <QtCore/QCoreApplication>
//#include <stdio.h>
//#include <tchar.h>
//#include <locale.h>
#include <windows.h>
//#include <wlanapi.h>
//#include <objbase.h>
#include <adhoc.h>
#include <iostream>
#include <OCIdl.h>

using namespace std;

// SSID du reseau :
#define ADHOC_SSID L"CAPTAIN_ADHOC"
// Mot de passe du reseau (statique pour le moment, mais c'est moyen secure...) :
#define ADHOC_PWD L"1234567890"
//L"1491_archibald_1941"



class cManagerSink : public IDot11AdHocManagerNotificationSink
{
public:
    HRESULT STDMETHODCALLTYPE OnNetworkAdd(IDot11AdHocNetwork *pIAdHocNetwork)
    {
        LPWSTR pSSID;
        pIAdHocNetwork->GetSSID(&pSSID);
        wprintf(L"[ManagerNotif] New network : %s\n", pSSID);
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnNetworkRemove(GUID * sig)
    {
        printf("[ManagerNotif] network removed\n");
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnInterfaceAdd(IDot11AdHocInterface *pIAdHocNetwork)
    {
        printf("[ManagerNotif] New interface\n");
        return S_OK;
    }

    HRESULT STDMETHODCALLTYPE OnInterfaceRemove(GUID * sig)
    {
        printf("[ManagerNotif] interface removed\n");
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef()
    {
        return 2;
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        return 1;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj)
    {
        HRESULT hr = S_OK;

        if (riid == IID_IUnknown)
        {
            *ppvObj = (IUnknown *)this;
        }
        else if (riid == IID_IDot11AdHocManagerNotificationSink)
        {
            *ppvObj = (IDot11AdHocManagerNotificationSink *)this;
        }
        else
        {
            hr = E_NOINTERFACE;
            *ppvObj = NULL;
        }

        return hr;
    }
};

class cSink : public IDot11AdHocNetworkNotificationSink
{
public:

    cSink(){}

    HRESULT __stdcall OnConnectFail(DOT11_ADHOC_CONNECT_FAIL_REASON reason)
    {
        printf("[NetworkNotif] Connection failed : ");
        switch(reason)
        {
        case DOT11_ADHOC_CONNECT_FAIL_DOMAIN_MISMATCH:
            printf("DOMAIN MISSMATCH\n");
            break;
        case DOT11_ADHOC_CONNECT_FAIL_PASSPHRASE_MISMATCH:
            printf("PASSPHRASE MISSMATCH\n");
            break;
        case DOT11_ADHOC_CONNECT_FAIL_OTHER:
            printf("OTHER\n");
            break;
        default:
            printf("UNKNOWN\n");
        }

        return S_OK;
    }

    HRESULT __stdcall OnStatusChange( DOT11_ADHOC_NETWORK_CONNECTION_STATUS status)
    {
        printf("[NetworkNotif] Status changed : ");
        switch (status)
        {
        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_INVALID:
            printf("INVALID\n");
            break;
        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_DISCONNECTED:
            printf("DISCONNECTED\n");
            break;
        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_CONNECTING:
            printf("CONNECTING\n");
            break;
        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_CONNECTED:
            printf("CONNECTED\n");
            break;
        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_FORMED:
            printf("FORMED\n");
            break;
        default:
            printf("UNKNOWN\n");
        }
        return S_OK;
    }


    ULONG STDMETHODCALLTYPE AddRef()
    {
        return 2;
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        return 1;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj)
    {
        HRESULT hr = S_OK;

        if (riid == IID_IUnknown)
        {
            *ppvObj = (IUnknown *)this;
        }
        else if (riid == IID_IDot11AdHocNetworkNotificationSink)
        {
            *ppvObj = (IDot11AdHocNetworkNotificationSink *)this;
        }
        else
        {
            hr = E_NOINTERFACE;
            *ppvObj = NULL;
        }

        return hr;
    }
};

cSink sink;

class cSecuritySettings : public IDot11AdHocSecuritySettings
{
public:
    cSecuritySettings(){}

    HRESULT __stdcall GetDot11AuthAlgorithm(DOT11_ADHOC_AUTH_ALGORITHM *pAuth)
    {
        *pAuth = DOT11_ADHOC_AUTH_ALGO_80211_OPEN;  //WEP
        //*pAuth = DOT11_ADHOC_AUTH_ALGO_RSNA_PSK;    //WPA2PSK
        return S_OK;
    }

    HRESULT __stdcall GetDot11CipherAlgorithm(DOT11_ADHOC_CIPHER_ALGORITHM *pCipher)
    {
        *pCipher = DOT11_ADHOC_CIPHER_ALGO_WEP;     //WEP
        //*pCipher = DOT11_ADHOC_CIPHER_ALGO_CCMP;    //WPA2PSK
        return S_OK;
    }

    ULONG STDMETHODCALLTYPE AddRef()
    {
        printf("addref!\n");
        return 2;
    }

    ULONG STDMETHODCALLTYPE Release()
    {
        printf("release!\n");
        return 1;
    }

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj)
    {
        HRESULT hr = S_OK;

        printf("query\n");
        if (riid == IID_IUnknown)
        {
            *ppvObj = (IUnknown *)this;
        }
        else if (riid == IID_IDot11AdHocSecuritySettings)
        {
            *ppvObj = (IDot11AdHocSecuritySettings *)this;
        }
        else
        {
            hr = E_NOINTERFACE;
            *ppvObj = NULL;
        }

        return hr;
    }
};

cSecuritySettings securitySettings;


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
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


    cManagerSink mSink;
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
        ans = pConnectionPoint->Advise((IUnknown*) &sink,&sinkCookie);
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

    if (!found)
    {
        printf("Creating the network... ");

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
        ans = pConnectionPoint->Advise((IUnknown*) &sink,&sinkCookie);
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


    return a.exec();
}
