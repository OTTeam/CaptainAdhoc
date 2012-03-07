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
#include "FileUtils.h"
#include "FolderDao.h"

static void printFileNames(QList<FileModel> list)
{
    foreach (FileModel file, list) {
        qDebug() << file.name();
    }
}

static bool createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "fileDb");
    db.setDatabaseName("files.sqlite");
    if (!db.open()) {
        qCritical(qPrintable(db.lastError().text()));
        return false;
    }
    return true;
}

static void testIndexing(QCoreApplication& a)
{
    QSqlDatabase db = QSqlDatabase::database("fileDb");

    if (!FileIndexDao::createTable(db, true)) {
        qCritical(qPrintable(db.lastError().text()));
        a.quit();
    }

    QTime time;
    time.start();

    FileIndexer indexer(db, true);
    indexer.addDirectory("C:/temp/test_sqlite/test_filesystem");
//    indexer.addDirectory("C:/Windows");
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
    if (result) {
        qDebug() << "file :" << file;
        qDebug() << "file (simple) : " << file.toSimpleFileModel();
    }

    list = indexer.searchFiles("fileindexer");
    printFileNames(list);

    printFileNames(indexer.searchFiles("*.cpp"));

    printFileNames(indexer.searchFiles("?ileindexer*"));

    printFileNames(indexer.searchFiles("?ndexer*"));

    printFileNames(indexer.searchFiles("test_filesystem"));

    printFileNames(indexer.searchFiles("*"));

    db.close();
}

static void testMd5Hash()
{
    QString path("E:/fr_visual_studio_2010_professional_x86_dvd_519327.iso");
    qDebug() << FileUtils::fileMd5Hash(path);
}

static void testDataStream()
{
    SimpleFileModel model(1, "test.exe", "exe", 100);
    qDebug() << "before write : " << model;
    QFile file("testDataStream");
    file.open(QFile::WriteOnly);

    QDataStream stream(&file);
    stream << model;

    file.close();
    file.open(QFile::ReadOnly);
    stream.setDevice(&file);
    SimpleFileModel inModel;
    stream >> inModel;

    qDebug() << "after read : " << inModel;

    //Test écriture liste
    FileIndexer indexer(QSqlDatabase::database("fileDb"), false);
    QList<SimpleFileModel> files = indexer.getSharedFiles();
    foreach (SimpleFileModel sfm, files) {
        qDebug() << sfm;
    }
    QFile testFileList("testFileList");
    testFileList.open(QFile::WriteOnly);
    stream.setDevice(&testFileList);
    stream << files;

    testFileList.close();
    testFileList.open(QFile::ReadOnly);
    stream.setDevice(&testFileList);
    QList<SimpleFileModel> newList;
    stream >> newList;
    qDebug() << "list size =" << newList.size();
    foreach (SimpleFileModel sfm, newList) {
        qDebug() << sfm;
    }
}

static void testFolderDao()
{
    QSqlDatabase db = QSqlDatabase::database("fileDb");

    if (!FolderDao::createTable(db, true)) {
        qDebug() << db.lastError().number() << db.lastError().text();
        qCritical(qPrintable(db.lastError().text()));
        return;
    }

    FolderModel folder;
    folder.setPath("C:/");

    FolderDao dao(db);
    qDebug() << "Insertion";
    dao.insertFolder(folder);
    qDebug() << folder;

    QList<FolderModel *> folders = dao.getAllFolders();
    qDebug() << *(folders.at(0));
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (!createDatabase())
        a.quit();

    QTime time;
    time.start();

//    testIndexing(a);
//    testMd5Hash();
//    testDataStream();
    testFolderDao();

    qDebug("Execution time : %fs", time.elapsed() / 1000.);

    return a.exec();
}
