#include "ManagerNotificationSink.h"
#include <QDebug>
#include <iostream>


ManagerNotificationSink::ManagerNotificationSink()
{
#ifdef TRACE
    qDebug() << "[CONS] ManagerNotificationSink";
#endif
}

ManagerNotificationSink::~ManagerNotificationSink()
{
#ifdef TRACE
    qDebug() << "[DEST] ManagerNotificationSink";
#endif
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnNetworkAdd(IDot11AdHocNetwork *pIAdHocNetwork)
{
    LPWSTR pSSID;
    pIAdHocNetwork->GetSSID(&pSSID);
    QString ssid = QString::fromWCharArray(pSSID);
    qDebug() << "[ManagerNotif] New network : " << ssid;
    emit NetworkAdded();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnNetworkRemove(GUID *)
{
    qDebug() << "[ManagerNotif] a network was removed";
    emit NetworkRemoved();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnInterfaceAdd(IDot11AdHocInterface *pIAdHocInterface)
{
    LPWSTR pIntName;
    pIAdHocInterface->GetFriendlyName(&pIntName);
    QString IntName = QString::fromWCharArray(pIntName);
    qDebug() << "[ManagerNotif] New interface :" << IntName;
    emit InterfaceAdded();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE ManagerNotificationSink::OnInterfaceRemove(GUID *)
{
    qDebug() << "[ManagerNotif] an interface was removed";
    emit InterfaceRemoved();
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
