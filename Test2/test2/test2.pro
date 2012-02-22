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

win32: LIBS += -L"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib/" -lwlanapi
win32: LIBS += -L"C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib/" -lole32

INCLUDEPATH += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include"
DEPENDPATH += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Include"

win32: PRE_TARGETDEPS += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib/wlanapi.lib"
win32: PRE_TARGETDEPS += "C:/Program Files (x86)/Microsoft SDKs/Windows/v7.0A/Lib/ole32.lib"
