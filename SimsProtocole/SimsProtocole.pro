#-------------------------------------------------
#
# Project created by QtCreator 2012-02-07T15:33:41
#
#-------------------------------------------------

QT       += core gui network

TARGET = SimsProtocole
TEMPLATE = app


SOURCES += main.cpp\
        MainWindow.cpp \
    ServeurTCP.cpp \
    GestionClients.cpp \
    Client.cpp

HEADERS  += MainWindow.h \
    ServeurTCP.h \
    utils.h \
    GestionClients.h \
    Client.h
