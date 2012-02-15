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

win32: LIBS += -L"$$PWD/../../../../../../Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lwlanapi
win32: LIBS += -L"$$PWD/../../../../../../Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lole32

INCLUDEPATH += "$$PWD/../../../../../../Program Files/Microsoft SDKs/Windows/v7.1/Include"
DEPENDPATH += "$$PWD/../../../../../../Program Files/Microsoft SDKs/Windows/v7.1/Include"

win32: PRE_TARGETDEPS += "$$PWD/../../../../../../Program Files/Microsoft SDKs/Windows/v7.1/Lib/wlanapi.lib"
win32: PRE_TARGETDEPS += "$$PWD/../../../../../../Program Files/Microsoft SDKs/Windows/v7.1/Lib/ole32.lib"
