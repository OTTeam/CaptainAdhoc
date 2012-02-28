#ifndef MANAGERNOTIFICATIONSINK_H
#define MANAGERNOTIFICATIONSINK_H

#include <windows.h>
#include <adhoc.h>
#include <QObject>

class ManagerNotificationSink : public QObject, public IDot11AdHocManagerNotificationSink
{
    Q_OBJECT

public:
    HRESULT STDMETHODCALLTYPE OnNetworkAdd(IDot11AdHocNetwork *pIAdHocNetwork);

    HRESULT STDMETHODCALLTYPE OnNetworkRemove(GUID * sig);

    HRESULT STDMETHODCALLTYPE OnInterfaceAdd(IDot11AdHocInterface *pIAdHocNetwork);

    HRESULT STDMETHODCALLTYPE OnInterfaceRemove(GUID * sig);

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj);

signals:
    void NetworkAdded();
    void NetworkRemoved();
    void InterfaceAdded();
    void InterfaceRemoved();
};

#endif // MANAGERNOTIFICATIONSINK_H
