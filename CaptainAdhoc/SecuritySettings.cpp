#include <iostream>
#include "SecuritySettings.h"


SecuritySettings::SecuritySettings(){}

HRESULT __stdcall SecuritySettings::GetDot11AuthAlgorithm(DOT11_ADHOC_AUTH_ALGORITHM *pAuth)
{
    *pAuth = DOT11_ADHOC_AUTH_ALGO_80211_OPEN;  //WEP
    //*pAuth = DOT11_ADHOC_AUTH_ALGO_RSNA_PSK;    //WPA2PSK
    return S_OK;
}

HRESULT __stdcall SecuritySettings::GetDot11CipherAlgorithm(DOT11_ADHOC_CIPHER_ALGORITHM *pCipher)
{
    *pCipher = DOT11_ADHOC_CIPHER_ALGO_WEP;     //WEP
    //*pCipher = DOT11_ADHOC_CIPHER_ALGO_CCMP;    //WPA2PSK
    return S_OK;
}

ULONG STDMETHODCALLTYPE SecuritySettings::AddRef()
{
    printf("addref!\n");
    return 2;
}

ULONG STDMETHODCALLTYPE SecuritySettings::Release()
{
    printf("release!\n");
    return 1;
}

HRESULT STDMETHODCALLTYPE SecuritySettings::QueryInterface(REFIID riid, PVOID *ppvObj)
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
