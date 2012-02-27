#include "fileindexdao.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

static const char * TABLE_NAME = "FILES";
static const char * COLUMN_ID = "id";
static const char * COLUMN_NAME = "name";
static const char * COLUMN_PATH = "path";

FileIndexDao::FileIndexDao(QSqlDatabase database) : _database(database), _insertQuery(database)
{
    _insertQuery.prepare(QString("INSERT INTO ") + TABLE_NAME + "("
                  + COLUMN_NAME + "," + COLUMN_PATH + ") VALUES (:name, :path);");
}

bool FileIndexDao::insertFile(FileModel& model)
{
    QSqlQuery q(_database);
    q.prepare("INSERT INTO FILES (name, base_dir, dir_path, path, index_time) VALUES (:name, :baseDir, :dirPath, :path, :indexTime);");

    q.bindValue(":path", model.path());
    q.bindValue(":baseDir", model.baseDir());
    q.bindValue(":dirPath", model.dirPath());
    q.bindValue(":name", model.name());
    q.bindValue(":indexTime", model.indexTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    if (q.exec()) {
        model.setId(q.lastInsertId().toInt(NULL));
        return true;
    }
    return false;
}

QList<FileModel> FileIndexDao::getAllFiles()
{
    QSqlQuery q("SELECT id, name, base_dir, dir_path, path FROM FILES", _database);
    q.exec();
    QList<FileModel> list;
    while (q.next()) {
        FileModel model;
        model.setId(q.value(0).toInt(NULL));
        model.setName(q.value(1).toString());
        model.setBaseDir(q.value(2).toString());
        model.setDirPath(q.value(3).toString());
        model.setPath(q.value(4).toString());
        model.setIndexTime(q.value(5).toDateTime());
        list << model;
    }
    return list;
}

QList<FileModel> FileIndexDao::searchFiles(const QString& keyword)
{
    QSqlQuery q("SELECT id, name, base_dir, dir_path, path, index_time FROM FILES WHERE name LIKE '%" + keyword + "%'", _database);
    q.exec();
    QList<FileModel> list;
    while (q.next()) {
        FileModel model;
        model.setId(q.value(0).toInt(NULL));
        model.setName(q.value(1).toString());
        model.setBaseDir(q.value(2).toString());
        model.setDirPath(q.value(3).toString());
        model.setPath(q.value(4).toString());
        model.setIndexTime(q.value(5).toDateTime());
        list << model;
    }
    return list;
}

bool FileIndexDao::getFile(int id, FileModel& model)
{
    QSqlQuery query(_database);
    query.prepare("SELECT id, name, base_dir, dir_path, path, index_time FROM FILES "
                  "WHERE id = :id");
    query.bindValue(":id", id);
    if (!query.exec()) {
        qDebug() << "Unable to execute query";
        return false;
    }
    if (!query.next()) {
        qDebug() << "No result";
        return false;
    }
    model.setId(query.value(0).toInt(NULL));
    model.setName(query.value(1).toString());
    model.setBaseDir(query.value(2).toString());
    model.setDirPath(query.value(3).toString());
    model.setPath(query.value(4).toString());
    model.setIndexTime(query.value(5).toDateTime());
    return true;
}

bool FileIndexDao::createTable(QSqlDatabase database, bool dropIfExists)
{
    QSqlQuery query(database);
    if (dropIfExists)
        query.exec("DROP TABLE IF EXISTS FILES");
    return query.exec("CREATE TABLE FILES (ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT, BASE_DIR TEXT, DIR_PATH TEXT, PATH TEXT, INDEX_TIME TEXT);") && createIndex(database);
}

bool FileIndexDao::createIndex(QSqlDatabase database)
{
    QSqlQuery query(database);
    return query.exec("CREATE UNIQUE INDEX PATH_IDX ON FILES (PATH ASC)");
}
