#ifndef FILEINDEXDAO_H
#define FILEINDEXDAO_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>
#include "FileModel.h"

class FileIndexDao
{
public:

    FileIndexDao(QSqlDatabase database);

    bool insertFile(FileModel& model);
    bool getFile(int id, FileModel& model);

    QList<FileModel> getAllFiles();

    QList<FileModel> searchFiles(const QString& keyword, bool wildcard);

    void setDatabase(QSqlDatabase db) { _database = db; }

    static bool createTable(QSqlDatabase database, bool dropIfExists = false);

private:

    static bool createIndex(QSqlDatabase database);

    QSqlDatabase _database;
};

#endif // FILEINDEXDAO_H
