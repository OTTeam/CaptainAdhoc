#include <QtCore/QCoreApplication>
#include <stdio.h>
#include <tchar.h>
#include <locale.h>
#include <windows.h>
#include <wlanapi.h>
#include <objbase.h>
#include <adhoc.h>
#include <iostream>
#include <OCIdl.h>


static void DisplayError(char * message, DWORD lastError)
{
   char *  buffer = NULL;
   if(message == NULL){message = _T("Error");}

   if(FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_ALLOCATE_BUFFER, NULL, lastError, 0, (LPTSTR)&buffer, 0, NULL) == 0)
   {
      if(GetLastError() == ERROR_NOT_ENOUGH_MEMORY)
      {
         _ftprintf(stderr, _T("%s : not enough memory\n"), message);
      }
      else
      {
         _ftprintf(stderr, _T("FormatMessage() failed : %lu\n"), GetLastError());
      }
   }
   else
   {
      _ftprintf(stderr, _T("%s : %s\n"), message, buffer);
      LocalFree(buffer), buffer = NULL;
   }
}

static void DisplayAuthAlgorithm(DOT11_AUTH_ALGORITHM authAlgorithm)
{
   char * algo = NULL;

   switch(authAlgorithm)
   {
      case DOT11_AUTH_ALGO_80211_OPEN :         algo = _T("IEEE 802.11 Open System"); break;
      case DOT11_AUTH_ALGO_80211_SHARED_KEY :   algo = _T("802.11 Shared Key"); break;
      case DOT11_AUTH_ALGO_WPA :                algo = _T("WPA"); break;
      case DOT11_AUTH_ALGO_WPA_PSK :            algo = _T("WPA-PSK"); break;
      case DOT11_AUTH_ALGO_WPA_NONE :           algo = _T("not supported"); break;
      case DOT11_AUTH_ALGO_RSNA :               algo = _T("802.11i RSNA (WPA2)"); break;
      case DOT11_AUTH_ALGO_RSNA_PSK :           algo = _T(" 802.11i RSNA PSK (WPA2-PSK)"); break;
   }

   if(algo == NULL)
   {
      if(authAlgorithm >= DOT11_AUTH_ALGO_IHV_START && authAlgorithm <= DOT11_AUTH_ALGO_IHV_END)
      {
         algo = _T("Proprietary auth algorithm");
      }
      else
      {
         algo = _T("Unknown algorithm");
      }
   }

   _tprintf(_T("\t\tAuth Algorithm is [%s]\n"), algo);
}

static char * PhysicalTypeToString(DOT11_PHY_TYPE physicalType)
{
   char *  type = NULL;

   switch(physicalType)
   {
      case dot11_phy_type_unknown :    type = _T("unknown or uninitialized PHY type"); break;
      case dot11_phy_type_fhss :       type = _T("frequency-hopping spread-spectrum (FHSS) PHY. Bluetooth devices can use FHSS or an adaptation of FHSS"); break;
      case dot11_phy_type_dsss :       type = _T("direct sequence spread spectrum (DSSS) PHY type"); break;
      case dot11_phy_type_irbaseband : type = _T("infrared (IR) baseband PHY type"); break;
      case dot11_phy_type_ofdm :       type = _T("orthogonal frequency division multiplexing (OFDM) PHY type. 802.11a devices can use OFDM"); break;
      case dot11_phy_type_hrdsss :     type = _T("high-rate DSSS (HRDSSS) PHY type"); break;
      case dot11_phy_type_erp :        type = _T("extended rate PHY type (ERP). 802.11g devices can use ERP"); break;
      case dot11_phy_type_ht :         type = _T("unknown or uninitialized PHY type"); break;
   }

   if(type == NULL)
   {
      if(physicalType >= dot11_phy_type_IHV_start && physicalType <= dot11_phy_type_IHV_end)
      {
         type = _T("Proprietary Physical Type");
      }
      else
      {
         type = _T("Unknown Physical Type");
      }
   }

   return type;
}

static char * StateToString(WLAN_INTERFACE_STATE state)
{
   char *  status = NULL;

   switch(state)
   {
      case wlan_interface_state_not_ready :              status = _T("not ready");              break;
      case wlan_interface_state_connected :              status = _T("connected");              break;
      case wlan_interface_state_ad_hoc_network_formed :  status = _T("No peer has connected");  break;
      case wlan_interface_state_disconnecting :          status = _T("disconnecting");          break;
      case wlan_interface_state_disconnected :           status = _T("disconnected");           break;
      case wlan_interface_state_associating :            status = _T("associating");            break;
      case wlan_interface_state_discovering :            status = _T("discovering");            break;
      case wlan_interface_state_authenticating :         status = _T("authenticating");         break;
      default :                                          status = _T("unknown state");          break;
   }

   return status;
}

static void DisplayBssType(DOT11_BSS_TYPE bssType)
{
   char * type;

   if(bssType == dot11_BSS_type_infrastructure)
   {
      type = _T("infastructure");
   }
   else if(bssType == dot11_BSS_type_independent)
   {
      type = _T("ad hoc");
   }
   else if(bssType == dot11_BSS_type_any)
   {
      type = _T("infastructure, ad hoc");
   }
   else
   {
      type = _T("unknown");
   }

   _tprintf(_T("\t\tNetwork type is [%s]\n"), type);
}

static void DisplayProfileName(BOOL hasProfile, LPCWSTR profileName)
{
   if(hasProfile)
   {
      _tprintf(_T("\t\tProfile name is %ws\n"), profileName);
   }
   else
   {
      _putts(_T("\t\tThere is no profile associated with this network"));
   }
}

static void DisplaySSID(PDOT11_SSID ssid)
{
   _tprintf(_T("\t\tSSID is [%.*hs]\n"), (int)(ssid->uSSIDLength), ssid->ucSSID);
}

static void DisplayBSSID(HANDLE clientHandle, const GUID * interfaceGuid)
{
   DWORD size = 0;
   PWLAN_CONNECTION_ATTRIBUTES attributes = NULL;
   DWORD lastError = WlanQueryInterface(clientHandle, interfaceGuid, wlan_intf_opcode_current_connection, NULL, &size, (PVOID*)&attributes, NULL);

   if(lastError == ERROR_SUCCESS)
   {
      PUCHAR bssid = attributes->wlanAssociationAttributes.dot11Bssid;
      _tprintf(_T("\t\tBSSID is %02X-%02X-%02X-%02X-%02X-%02X\n"),
               bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5]);
      WlanFreeMemory(attributes), attributes = NULL;
   }
   else
   {
      DisplayError(_T("Error in the WlanQueryInterface() function call"), lastError);
   }
}

static void DisplayConnectability(BOOL isConnectable, WLAN_REASON_CODE reasonCode)
{
   if(isConnectable)
   {
      _putts(_T("\t\tNetwork is connectable"));
   }
   else
   {
      WCHAR reason[128];
      DWORD lastError = WlanReasonCodeToString(reasonCode, _countof(reason), reason, NULL);

      if(lastError == ERROR_SUCCESS)
      {
         _tprintf(_T("\t\tNetwork is not connectable : %ws\n"), reason);
      }
      else
      {
         DisplayError(_T("Error in the WlanReasonCodeToString() function call"), lastError);
      }
   }
}

static void DisplayPhyTypes(HANDLE clientHandle, const GUID * interfaceGuid, WLAN_AVAILABLE_NETWORK * info)
{
   ULONG type = 0;
   PWLAN_BSS_LIST list = NULL;
   DWORD lastError;

   if(info->uNumberOfPhyTypes > 1)
   {
      _tprintf(_T("\t\tThere are %lu physical types supported :"), info->uNumberOfPhyTypes);
   }
   else
   {
      _putts(_T("\t\tThere is 1 physical types supported :"));
   }

   for(; type < info->uNumberOfPhyTypes; type++)
   {
      _tprintf(_T("\t\t\t- %s\n"), PhysicalTypeToString(info->dot11PhyTypes[type]));
   }

   lastError = WlanGetNetworkBssList(clientHandle, interfaceGuid, &info->dot11Ssid, info->dot11BssType,
                                     info->bSecurityEnabled, NULL, &list);

   if(lastError == ERROR_SUCCESS)
   {

      if(list->dwNumberOfItems == 1)
      {
         _tprintf(_T("\t\tPhysical type used for this network : %s\n"),
                  PhysicalTypeToString(list->wlanBssEntries[0].dot11BssPhyType));
      }

       WlanFreeMemory(list), list = NULL;
   }
   else if(lastError != ERROR_NOT_SUPPORTED)
   {
      DisplayError(_T("Error in the WlanGetNetworkBssList() function call"), lastError);
   }
}

static void DisplayStatus(BOOL isConnected)
{
   _tprintf(_T("\t\tStatus : %s\n"), isConnected ? _T("connected") : _T("disconnected"));
}

static void DisplaySecurity(BOOL isSecure, DOT11_CIPHER_ALGORITHM cipherAlgorithm)
{
   if(isSecure)
   {
       char * algo = NULL;

      switch(cipherAlgorithm)
      {
         case DOT11_CIPHER_ALGO_NONE :          algo = _T("No cipher");                   break;
         case DOT11_CIPHER_ALGO_WEP40 :         algo = _T("WEP (40 bits cipher key)");    break;
         case DOT11_CIPHER_ALGO_TKIP :          algo = _T("TKIP");                        break;
         case DOT11_CIPHER_ALGO_CCMP :          algo = _T("AES-CCMP");                    break;
         case DOT11_CIPHER_ALGO_WEP104 :        algo = _T("WEP (104 bits cipher key)");   break;
         case DOT11_CIPHER_ALGO_WEP :           algo = _T("WEP");                         break;
         case DOT11_CIPHER_ALGO_WPA_USE_GROUP : algo = _T("Use Group Key cipher suite");  break;
      }

      if(algo == NULL)
      {
         if(cipherAlgorithm >= DOT11_CIPHER_ALGO_IHV_START && cipherAlgorithm <= DOT11_CIPHER_ALGO_IHV_END)
         {
            algo = _T("Proprietary cipher algorithm");
         }
         else
         {
            algo = _T("Unknown algorithm");
         }
      }

      _tprintf(_T("\t\tSecurity is enbled : %s\n"), algo);
   }
   else
   {
      _putts(_T("\t\tSecurity is disabled"));
   }
}

static void DisplaySignalStrength(WLAN_SIGNAL_QUALITY quality)
{
   _tprintf(_T("\t\tSignal strength is %lu%%\n"), quality);
}

static void EnumerateNetwork(HANDLE clientHandle, const GUID * interfaceGuid)
{
   WLAN_AVAILABLE_NETWORK_LIST * networkList = NULL;
   DWORD lastError = WlanGetAvailableNetworkList(clientHandle, interfaceGuid, 0, NULL, &networkList);

   if(lastError != ERROR_SUCCESS)
   {
//      DisplayError(_T("Error in the WlanGetAvailableNetworkList() function call"), lastError);
   }
   else
   {
      _tprintf(_T("\tThere are %lu Wifi network\n"), networkList->dwNumberOfItems);

      for(networkList->dwIndex = 0; networkList->dwIndex != networkList->dwNumberOfItems; networkList->dwIndex++)
      {
         WLAN_AVAILABLE_NETWORK * info = &networkList->Network[networkList->dwIndex];

         _tprintf(_T("\n\t\tNetwork %lu\n"), networkList->dwIndex);
         DisplayStatus(info->dwFlags & WLAN_AVAILABLE_NETWORK_CONNECTED);
         DisplayProfileName(info->dwFlags & WLAN_AVAILABLE_NETWORK_HAS_PROFILE, info->strProfileName);
         DisplaySSID(&info->dot11Ssid);
         DisplayBssType(info->dot11BssType);
         DisplayBSSID(clientHandle, interfaceGuid);
//         DisplayConnectability(info->bNetworkConnectable, info->wlanNotConnectableReason);
//         DisplayPhyTypes(clientHandle, interfaceGuid, info);
//         DisplaySecurity(info->bSecurityEnabled, info->dot11DefaultCipherAlgorithm);
//         DisplaySignalStrength(info->wlanSignalQuality);
//         DisplayAuthAlgorithm(info->dot11DefaultAuthAlgorithm);
      }

      WlanFreeMemory(networkList), networkList = NULL;
   }
}

static void EnumerateInterface(HANDLE clientHandle)
{
   WLAN_INTERFACE_INFO_LIST * interfaceList = NULL;
   DWORD lastError = WlanEnumInterfaces(clientHandle, NULL, &interfaceList);

   if(lastError != ERROR_SUCCESS)
   {
//      DisplayError(_T("Error in the WlanEnumInterfaces() function call"), lastError);
   }
   else
   {
      DWORD item = 0;
      _tprintf(_T("There are %d Wifi interface\n"), interfaceList->dwNumberOfItems);

      for(; item != interfaceList->dwNumberOfItems; item++)
      {
         WLAN_INTERFACE_INFO * info = &interfaceList->InterfaceInfo[item];
         GUID * guid = &info->InterfaceGuid;
         LPOLESTR guid_str = NULL;

         if(StringFromCLSID(*guid, &guid_str) == S_OK)
         {
            _tprintf(_T("Interface %lu\n")
                     _T("\tThe Wifi interface GUID is [%ws]\n")
                     _T("\tThe Wifi interface description is [%ws]\n")
                     _T("\tThe Wifi interface state is [%s]\n"),
                     item, guid_str, info->strInterfaceDescription, "state");

            CoTaskMemFree(guid_str), guid_str = NULL;
         }
         else
         {
//            DisplayError(_T("Error in the StringFromCLSID() function call"), ERROR_NOT_ENOUGH_MEMORY);
         }

         EnumerateNetwork(clientHandle, guid);
      }

      WlanFreeMemory(interfaceList), interfaceList = NULL;
   }
}

class cSink : public IDot11AdHocNetworkNotificationSink
{
public:

    cSink(){}

    HRESULT __stdcall OnConnectFail(DOT11_ADHOC_CONNECT_FAIL_REASON reason)
    {
        printf("Connection failed !\n");
        return S_OK;
    }

    HRESULT __stdcall OnStatusChange( DOT11_ADHOC_NETWORK_CONNECTION_STATUS status)
    {
        printf("TEST OK \n\n");
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

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
//    DWORD negotiatedVersion = 0;
//    HANDLE clientHandle = NULL;
//    DWORD lastError;
    IDot11AdHocManager * AdHocManager = NULL;
    IEnumDot11AdHocNetworks * networks = NULL;
    HRESULT ans;
    ULONG cnt;
    IDot11AdHocNetwork * network [5];
    IConnectionPointContainer  * pConnectionPointContainer;
    IConnectionPoint * pConnectionPoint;
    LPWSTR ssid;
    DWORD cPoint = NULL;

    std::cout << "Initialising COM Lib ...";
    ans = CoInitialize(NULL);

    std::cout << "OK" << std::endl << "Creating AdHocManager ...";
    ans = CoCreateInstance(CLSID_Dot11AdHocManager,NULL,CLSCTX_INPROC_SERVER,IID_IDot11AdHocManager,(void**) &AdHocManager);

    std::cout << "OK" << std::endl << "Getting Network list ...";
    ans = AdHocManager->GetIEnumDot11AdHocNetworks(NULL,&networks);

    std::cout << "OK" << std::endl  << "Extracting Networks...";
    ans = networks->Next(5,network,&cnt);

    printf("Got %d networks\n",cnt);


//    ans = AdHocManager->QueryInterface(IID_IConnectionPointContainer,(void**) &pConnectionPointContainer);
//    if (!SUCCEEDED(ans)) printf ("BANG !");

//    ans = pConnectionPointContainer->FindConnectionPoint(IID_IDot11AdHocInterface,&pConnectionPoint);
//    if (!SUCCEEDED(ans)) printf ("BANG 2!\n");
//    switch (ans)
//    {
//    case S_OK:
//        printf("OK");
//        break;
//    case REGDB_E_CLASSNOTREG:
//        printf("REGDB");
//        break;
//    case CLASS_E_NOAGGREGATION:
//        printf("CLASS");
//        break;
//    case E_NOINTERFACE:
//        printf("NOINTERFACE");
//        break;
//    case E_POINTER:
//        printf("POINTER");
//        break;
//    case CONNECT_E_NOCONNECTION :
//        printf("NOCONNECTION");
//        break;
//    default :
//        printf("%d",ans);
//    }
//    std::cout << std::endl;

//    if (cPoint == NULL) printf("NULL before\n");

//    std::cout << "Registering for notification ...";
//    ans = ((IConnectionPoint*)AdHocManager)->Advise((IUnknown *) &sink, &cPoint);
//    switch (ans)
//    {
//    case S_OK:
//        printf("OK");
//        break;
//    case REGDB_E_CLASSNOTREG:
//        printf("REGDB");
//        break;
//    case CLASS_E_NOAGGREGATION:
//        printf("CLASS");
//        break;
//    case E_NOINTERFACE:
//        printf("NOINTERFACE");
//        break;
//    case E_POINTER:
//        printf("POINTER");
//        break;
//    default :
//        printf("%d",ans);
//    }
//    std::cout << std::endl;

//    if (cPoint == NULL) printf("NULL\n");

    bool found = false;

    for (ULONG i = 0; i < cnt; i++)
    {
    network[i]->GetSSID(&ssid);
    std::wstring str(ssid);
    std::wcout << "ssid = " << str << std::endl;
    if (str==L"captainAdHoc")
    {
        found=true;
        printf("Connecting...");
        ans = network[i]->Connect(L"",0x54,false,false);
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
//    wprintf(ssid);

    }

    IDot11AdHocNetwork * newNet;
    if (!found)
    {
        printf("Creating the network...");

        ans = AdHocManager->CreateNetwork(L"captainAdHoc",L"",0x54,NULL,NULL,NULL,&newNet);
        ans = AdHocManager->CommitCreatedNetwork(newNet,false,false);
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


//    _tsetlocale(LC_ALL, _T(""));

//    lastError = WlanOpenHandle(WLAN_API_VERSION, NULL, &negotiatedVersion, &clientHandle); /* open the Wifi box */

//    if(lastError != ERROR_SUCCESS)
//    {
////       DisplayError(_T("Error in the WlanOpenHandle() function call"), lastError);
//    }
//    else
//    {
//       _tprintf(_T("Asked version to WLAN interface is %lu, negociated version is %lu\n"),
//                WLAN_API_VERSION, negotiatedVersion);

//       EnumerateInterface(clientHandle);

//       lastError = WlanCloseHandle(clientHandle, NULL);

//       if(lastError != ERROR_SUCCESS)
//       {
////          DisplayError(_T("Error in the WlanCloseHandle() function call"), lastError);
//       }
//    }

    return a.exec();
}
