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

win32: LIBS += -L"C:/Microsoft SDK 7/Lib/" -lwlanapi
win32: LIBS += -L"C:/Microsoft SDK 7/Lib/" -lole32

INCLUDEPATH += "C:/Microsoft SDK 7/Include"
DEPENDPATH += "C:/Microsoft SDK 7/Include"

win32: PRE_TARGETDEPS += "C:/Microsoft SDK 7/Lib/wlanapi.lib"
win32: PRE_TARGETDEPS += "C:/Microsoft SDK 7/Lib/ole32.lib"
