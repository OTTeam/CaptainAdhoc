#ifndef MANAGERNOTIFICATIONSINK_H
#define MANAGERNOTIFICATIONSINK_H

#include <windows.h>
#include <adhoc.h>

class ManagerNotificationSink : public IDot11AdHocManagerNotificationSink
{
public:
    HRESULT STDMETHODCALLTYPE OnNetworkAdd(IDot11AdHocNetwork *pIAdHocNetwork);

    HRESULT STDMETHODCALLTYPE OnNetworkRemove(GUID * sig);

    HRESULT STDMETHODCALLTYPE OnInterfaceAdd(IDot11AdHocInterface *pIAdHocNetwork);

    HRESULT STDMETHODCALLTYPE OnInterfaceRemove(GUID * sig);

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj);
};

#endif // MANAGERNOTIFICATIONSINK_H
