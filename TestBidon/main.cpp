#define _M_IX86
#include <QtCore/QCoreApplication>
#include <windows.h>
#include <wlanapi.h>




int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    DWORD negotiatedVersion = 0;
    HANDLE clientHandle = NULL;
    DWORD lastError;

    lastError = WlanOpenHandle(WLAN_API_VERSION, NULL, &negotiatedVersion, &clientHandle); /* open the Wifi box */

    if(lastError != ERROR_SUCCESS)
    {
        printf("error");
    }
    else
    {
        printf("OK");
    }


    return a.exec();
}
