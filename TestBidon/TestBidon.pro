#-------------------------------------------------
#
# Project created by QtCreator 2012-02-08T11:01:13
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = TestBidon
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp


win32: LIBS += "-L$$PWD/../../../../Program Files/Microsoft SDKs/Windows/v7.1/Lib/" -lwlanapi

INCLUDEPATH += "$$PWD/../../../../Program Files/Microsoft SDKs/Windows/v7.1/Include" / "C:\Program Files\Microsoft Visual Studio 10.0\VC\include"
DEPENDPATH += "$$PWD/../../../../Program Files/Microsoft SDKs/Windows/v7.1/Include"

win32: PRE_TARGETDEPS += "$$PWD/../../../../Program Files/Microsoft SDKs/Windows/v7.1/Lib/wlanapi.lib"
