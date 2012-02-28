#include "ManagerNotificationSink.h"
#include <iostream>


HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnNetworkAdd(IDot11AdHocNetwork *pIAdHocNetwork)
{
    LPWSTR pSSID;
    pIAdHocNetwork->GetSSID(&pSSID);
    wprintf(L"[ManagerNotif] New network : %s\n", pSSID);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnNetworkRemove(GUID * sig)
{
    printf("[ManagerNotif] network removed\n");
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnInterfaceAdd(IDot11AdHocInterface *pIAdHocNetwork)
{
    printf("[ManagerNotif] New interface\n");
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnInterfaceRemove(GUID * sig)
{
    printf("[ManagerNotif] interface removed\n");
    return S_OK;
}

ULONG STDMETHODCALLTYPE ManagerNotificationSink::AddRef()
{
    return 2;
}

ULONG STDMETHODCALLTYPE ManagerNotificationSink::Release()
{
    return 1;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::QueryInterface(REFIID riid, PVOID *ppvObj)
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
