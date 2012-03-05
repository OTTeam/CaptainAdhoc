#ifndef NETWORKNOTIFICATIONSINK_H
#define NETWORKNOTIFICATIONSINK_H

#include <windows.h>
#include <adhoc.h>
#include <QObject>

enum CONNECTION_STATUS  { FORMED, CONNECTED, DISCONNECTED };
enum CONNECTION_FAIL    { DOMAIN_MISMATCH, PWD_MISMATCH, OTHER };

class NetworkNotificationSink : public QObject, public IDot11AdHocNetworkNotificationSink
{
    Q_OBJECT

signals:
    void ConnectionStatusChanged(int);
    void ConnectionFail(int);

public:
    NetworkNotificationSink();
    virtual ~NetworkNotificationSink();

    HRESULT __stdcall OnConnectFail(DOT11_ADHOC_CONNECT_FAIL_REASON reason);

    HRESULT __stdcall OnStatusChange( DOT11_ADHOC_NETWORK_CONNECTION_STATUS status);

    ULONG STDMETHODCALLTYPE AddRef();
    ULONG STDMETHODCALLTYPE Release();
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj);
};

#endif // NETWORKNOTIFICATIONSINK_H
