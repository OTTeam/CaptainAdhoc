#ifndef SECURITYSETTINGS_H
#define SECURITYSETTINGS_H

#include <windows.h>
#include <adhoc.h>

class SecuritySettings : public IDot11AdHocSecuritySettings
{
public:
    SecuritySettings();

    HRESULT __stdcall GetDot11AuthAlgorithm(DOT11_ADHOC_AUTH_ALGORITHM *pAuth);

    HRESULT __stdcall GetDot11CipherAlgorithm(DOT11_ADHOC_CIPHER_ALGORITHM *pCipher);

    ULONG STDMETHODCALLTYPE AddRef();

    ULONG STDMETHODCALLTYPE Release();

    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, PVOID *ppvObj);
};

#endif // SECURITYSETTINGS_H
