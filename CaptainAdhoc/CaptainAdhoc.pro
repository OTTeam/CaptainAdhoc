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
    WifiManager.cpp

HEADERS  += MainWindow.h \
    ServeurTCP.h \
    utils.h \
    GestionClients.h \
    Client.h \
    ClientDiscovery.h \
    WifiManager.h


win32: LIBS += -L"C:/Microsoft SDK 7/Lib/" -lwlanapi
win32: LIBS += -L"C:/Microsoft SDK 7/Lib/" -lole32

INCLUDEPATH += "C:/Microsoft SDK 7/Include"
DEPENDPATH += "C:/Microsoft SDK 7/Include"

win32: PRE_TARGETDEPS += "C:/Microsoft SDK 7/Lib/wlanapi.lib"
win32: PRE_TARGETDEPS += "C:/Microsoft SDK 7/Lib/ole32.lib"
