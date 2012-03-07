#ifndef FOLDERDAO_H
#define FOLDERDAO_H

#include <QSqlDatabase>
#include "FolderModel.h"

class FolderDao
{
public:
    FolderDao(QSqlDatabase database);

    bool insertFolder(FolderModel& folder);

    QList<FolderModel *> getAllFolders();

    void setDatabase(QSqlDatabase db) { _database = db; }

    static bool createTable(QSqlDatabase database, bool dropIfExists = false);

private:

    QSqlDatabase _database;
};

#endif // FOLDERDAO_H
