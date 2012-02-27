#include <QtCore/QCoreApplication>
#include <QDir>
#include <QFile>
#include <QSqlDatabase>
#include <QtGlobal>
#include <QDebug>
#include <QSqlError>
#include <QTime>

#include <iostream>

#include "FileIndexer.h"

static void printFileNames(QList<FileModel> list)
{
    foreach (FileModel file, list) {
        qDebug() << file.name();
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "fileDb");
    db.setDatabaseName("files.sqlite");
    if (!db.open()) {
        qCritical(qPrintable(db.lastError().text()));
        a.quit();
    }

    if (!FileIndexDao::createTable(db, false)) {
        qCritical(qPrintable(db.lastError().text()));
        a.quit();
    }

    QTime time;
    time.start();

    FileIndexer indexer(db);
    indexer.addDirectory("C:/temp/test_sqlite/test_filesystem");
//    indexer.addDirectory("C:/Qt/2009.02/qt/src/sql");
//    QStringList nameFilters;
//    nameFilters << "*.cpp" << "*.h";
//    indexer.setNameFilters(nameFilters);
    qint32 count = indexer.updateDatabase();

    qDebug() << "Indexed " << count << " files in" << time.elapsed() << "ms";

    QList<FileModel> list = indexer.getAllIndexedFiles();
    qDebug() << "Total number of indexed files :" << list.size();

    FileIndexDao dao(db);

    FileModel file;
    bool result = dao.getFile(1, file);
    if (result)
        qDebug() << "file :" << file;

    list = indexer.searchFiles("fileindexer");
    printFileNames(list);

    printFileNames(indexer.searchFiles("*.cpp"));

    printFileNames(indexer.searchFiles("?ileindexer*"));

    printFileNames(indexer.searchFiles("?ndexer*"));

    printFileNames(indexer.searchFiles("test_filesystem"));

    printFileNames(indexer.searchFiles("*"));

    db.close();
    return a.exec();
}
