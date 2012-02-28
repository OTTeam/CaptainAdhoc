#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T15:33:41
#
#-------------------------------------------------

QT       += core gui network

TARGET = CaptainAdhoc
TEMPLATE = app


SOURCES += main.cpp\
    MainWindow.cpp \
    ServeurTCP.cpp \
    GestionClients.cpp \
    Client.cpp \
    ClientDiscovery.cpp \
    WifiManager.cpp \
    ManagerNotificationSink.cpp \
    NetworkNotificationSink.cpp \
    SecuritySettings.cpp \
    WifiInterface.cpp

HEADERS  += MainWindow.h \
    ServeurTCP.h \
    utils.h \
    GestionClients.h \
    Client.h \
    ClientDiscovery.h \
    WifiManager.h \
    ManagerNotificationSink.h \
    NetworkNotificationSink.h \
    SecuritySettings.h \
    WifiInterface.h


win32: LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lwlanapi
win32: LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lole32

INCLUDEPATH += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Include"
DEPENDPATH += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Include"

win32: PRE_TARGETDEPS += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/wlanapi.lib"
win32: PRE_TARGETDEPS += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/ole32.lib"
