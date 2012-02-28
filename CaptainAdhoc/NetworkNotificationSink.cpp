#include "NetworkNotificationSink.h"


NetworkNotificationSink::NetworkNotificationSink(){}

HRESULT __stdcall NetworkNotificationSink::OnConnectFail(DOT11_ADHOC_CONNECT_FAIL_REASON reason)
{
//    printf("[NetworkNotif] Connection failed : ");
    switch(reason)
    {
    case DOT11_ADHOC_CONNECT_FAIL_DOMAIN_MISMATCH:
//        printf("DOMAIN MISSMATCH\n");
        break;
    case DOT11_ADHOC_CONNECT_FAIL_PASSPHRASE_MISMATCH:
//        printf("PASSPHRASE MISSMATCH\n");
        break;
    case DOT11_ADHOC_CONNECT_FAIL_OTHER:
//        printf("OTHER\n");
        break;
//    default:
//        printf("UNKNOWN\n");
        ;
    }

    return S_OK;
}

HRESULT __stdcall NetworkNotificationSink::OnStatusChange( DOT11_ADHOC_NETWORK_CONNECTION_STATUS status)
{
//    printf("[NetworkNotif] Status changed : ");
    switch (status)
    {
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_FORMED:
//        emit ConnectionStatusChanged(FORMED);
        break;
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_CONNECTED:
//        emit ConnectionStatusChanged(CONNECTED);
        break;
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_DISCONNECTED:
//        emit ConnectionStatusChanged(DISCONNECTED);
        break;
//        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_INVALID:
//            printf("INVALID\n");
//            break;
//        case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_CONNECTING:
//            printf("CONNECTING\n");
//            break;
//        default:
//            printf("UNKNOWN\n");
    }
    return S_OK;
}


ULONG STDMETHODCALLTYPE NetworkNotificationSink::AddRef()
{
    return 2;
}

ULONG STDMETHODCALLTYPE NetworkNotificationSink::Release()
{
    return 1;
}

HRESULT STDMETHODCALLTYPE NetworkNotificationSink::QueryInterface(REFIID riid, PVOID *ppvObj)
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
