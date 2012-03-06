#include "FileIndexDao.h"
#include <QSqlQuery>
#include <QVariant>
#include <QDebug>

static const char * COLUMN_LIST = "name, type, base_dir, dir_path, path, index_time, size, hash";

FileIndexDao::FileIndexDao(QSqlDatabase database) : _database(database)
{
}

bool FileIndexDao::insertFile(FileModel& model)
{
    QSqlQuery q(_database);
    q.prepare(QString("INSERT INTO FILES (") + COLUMN_LIST + ") VALUES (:name, :type, :baseDir, :dirPath, :path, :indexTime, :size, :hash);");

    q.bindValue(":path", model.path());
    q.bindValue(":baseDir", model.baseDir());
    q.bindValue(":dirPath", model.dirPath());
    q.bindValue(":name", model.name());
    q.bindValue(":type", model.type());
    q.bindValue(":indexTime", model.indexTime().toString("yyyy-MM-dd hh:mm:ss.zzz"));
    q.bindValue(":size", model.size());
    q.bindValue(":hash", model.hash());
    if (q.exec()) {
        model.setId(q.lastInsertId().toInt(NULL));
        return true;
    }
    return false;
}

QList<FileModel> FileIndexDao::getAllFiles()
{
    QSqlQuery q(QString("SELECT id, ") + COLUMN_LIST + " FROM FILES", _database);
    q.exec();
    QList<FileModel> list;
    while (q.next()) {
        FileModel model;
        model.setId(q.value(0).toInt(NULL));
        model.setName(q.value(1).toString());
        model.setType(q.value(2).toString());
        model.setBaseDir(q.value(3).toString());
        model.setDirPath(q.value(4).toString());
        model.setPath(q.value(5).toString());
        model.setIndexTime(q.value(6).toDateTime());
        model.setSize(q.value(7).toInt(NULL));
        model.setHash(q.value(8).toString());
        list << model;
    }
    return list;
}

QList<FileModel> FileIndexDao::searchFiles(const QString& keyword, bool wildcard)
{
    QString queryString = QString("SELECT id, ") + COLUMN_LIST + " FROM FILES WHERE name LIKE '";
    if (!wildcard)
        queryString.append("%");
    queryString.append(keyword);
    if (!wildcard)
        queryString.append("%");
    queryString.append("' ESCAPE '/'");

    QSqlQuery q(queryString, _database);
    q.exec();
    QList<FileModel> list;
    while (q.next()) {
        FileModel model;
        model.setId(q.value(0).toInt(NULL));
        model.setName(q.value(1).toString());
        model.setType(q.value(2).toString());
        model.setBaseDir(q.value(3).toString());
        model.setDirPath(q.value(4).toString());
        model.setPath(q.value(5).toString());
        model.setIndexTime(q.value(6).toDateTime());
        model.setSize(q.value(7).toInt(NULL));
        model.setHash(q.value(8).toString());
        list << model;
    }
    return list;
}

bool FileIndexDao::getFile(int id, FileModel& model)
{
    QSqlQuery query(_database);
    query.prepare(QString("SELECT id, ") + COLUMN_LIST + " FROM FILES "
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
    model.setType(query.value(2).toString());
    model.setBaseDir(query.value(3).toString());
    model.setDirPath(query.value(4).toString());
    model.setPath(query.value(5).toString());
    model.setIndexTime(query.value(6).toDateTime());
    model.setSize(query.value(7).toInt(NULL));
    model.setHash(query.value(8).toString());
    return true;
}

bool FileIndexDao::createTable(QSqlDatabase database, bool dropIfExists)
{
    QSqlQuery query(database);
    if (dropIfExists)
        query.exec("DROP TABLE IF EXISTS FILES");
    return query.exec("CREATE TABLE FILES (ID INTEGER PRIMARY KEY AUTOINCREMENT, NAME TEXT, TYPE TEXT, BASE_DIR TEXT, DIR_PATH TEXT, PATH TEXT, INDEX_TIME TEXT, SIZE INTEGER, HASH TEXT);") && createIndex(database);
}

bool FileIndexDao::createIndex(QSqlDatabase database)
{
    QSqlQuery query(database);
    return query.exec("CREATE UNIQUE INDEX PATH_IDX ON FILES (PATH ASC)");
}
