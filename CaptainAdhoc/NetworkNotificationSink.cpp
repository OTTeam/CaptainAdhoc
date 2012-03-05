#include "NetworkNotificationSink.h"

#include <QDebug>


NetworkNotificationSink::NetworkNotificationSink()
{
#ifdef TRACE
    qDebug() << "[CONS] NetworkNotificationSink";
#endif
}

NetworkNotificationSink::~NetworkNotificationSink()
{
#ifdef TRACE
    qDebug() << "[DEST] NetworkNotificationSink";
#endif
}

HRESULT __stdcall NetworkNotificationSink::OnConnectFail(DOT11_ADHOC_CONNECT_FAIL_REASON reason)
{
    switch(reason)
    {
    case DOT11_ADHOC_CONNECT_FAIL_DOMAIN_MISMATCH:
        emit ConnectionFail(DOMAIN_MISMATCH);
        break;
    case DOT11_ADHOC_CONNECT_FAIL_PASSPHRASE_MISMATCH:
        emit ConnectionFail(PWD_MISMATCH);
        break;
    case DOT11_ADHOC_CONNECT_FAIL_OTHER:
        emit ConnectionFail(OTHER);
        break;
    }

    return S_OK;
}

HRESULT __stdcall NetworkNotificationSink::OnStatusChange(DOT11_ADHOC_NETWORK_CONNECTION_STATUS status)
{
    switch (status)
    {
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_FORMED:
        qDebug() << "Notification sent : network formed";
        emit ConnectionStatusChanged(FORMED);
        break;
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_CONNECTED:
        qDebug() << "Notification sent : connected to network";
        emit ConnectionStatusChanged(CONNECTED);
        break;
    case DOT11_ADHOC_NETWORK_CONNECTION_STATUS_DISCONNECTED:
        qDebug() << "Notification sent : disconnected from network";
        emit ConnectionStatusChanged(DISCONNECTED);
        break;
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
