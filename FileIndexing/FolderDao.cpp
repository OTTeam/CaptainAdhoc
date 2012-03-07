#include "FolderDao.h"
#include <QSqlQuery>
#include <QVariant>
#include "FolderModel.h"

static const char * COLUMN_LIST = "path";

FolderDao::FolderDao(QSqlDatabase database) : _database(database)
{
}

bool FolderDao::insertFolder(FolderModel& folder)
{
    QSqlQuery query(QString("INSERT INTO FOLDERS (") + COLUMN_LIST + ") VALUES (:path)", _database);

    query.bindValue(":path", folder.path());

    if (query.exec()) {
        folder.setId(query.lastInsertId().toInt(NULL));
        return true;
    }
    return false;
}

QList<FolderModel *> FolderDao::getAllFolders()
{
    QSqlQuery query(QString("SELECT id, ") + COLUMN_LIST + " FROM FOLDERS", _database);

    QList<FolderModel *> folders;

    query.exec();

    while (query.next()) {
        FolderModel * model = new FolderModel;
        model->setId(query.value(0).toInt(NULL));
        model->setPath(query.value(1).toString());

        folders << model;
    }

    return folders;
}

bool FolderDao::createTable(QSqlDatabase database, bool dropIfExists)
{
    QSqlQuery query(database);
    if (dropIfExists)
        query.exec("DROP TABLE IF EXISTS FOLDERS");
    return query.exec("CREATE TABLE FOLDERS ( id INTEGER PRIMARY KEY AUTOINCREMENT, path TEXT )");
}
