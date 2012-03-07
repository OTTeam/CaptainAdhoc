# -------------------------------------------------
# Project created by QtCreator 2012-02-14T10:34:50
# -------------------------------------------------
QT -= gui
QT += sql
TARGET = file_indexing
CONFIG += console
CONFIG -= app_bundle
TEMPLATE = app
SOURCES += main.cpp \
    FileIndexDao.cpp \
    FileModel.cpp \
    FileIndexer.cpp \
    FileUtils.cpp \
    SimpleFileModel.cpp
HEADERS += FileIndexDao.h \
    FileModel.h \
    FileIndexer.h \
    FileUtils.h \
    SimpleFileModel.h
