#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T10:22:30
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = test2
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp

win32: LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lwlanapi
win32: LIBS += -L"C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lole32

INCLUDEPATH += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Include"
DEPENDPATH += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Include"

win32: PRE_TARGETDEPS += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/wlanapi.lib"
win32: PRE_TARGETDEPS += "C:/Program Files/Microsoft SDKs/Windows/v7.1/Lib/ole32.lib"
