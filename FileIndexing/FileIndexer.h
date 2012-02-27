#ifndef FILEINDEXER_H
#define FILEINDEXER_H

#include <QDir>
#include <QList>
#include "fileindexdao.h"


class FileIndexer
{
public:
    FileIndexer();
    FileIndexer(QSqlDatabase db);

    bool addDirectory(QDir dir);
    bool addDirectory(const QString& path);

    qint32 updateDatabase();

    QList<FileModel> getAllIndexedFiles();

    QList<FileModel> searchFiles(const QString& keyword);

    void setDatabase(QSqlDatabase db) { _dao.setDatabase(db); }

    void setNameFilters(const QStringList& nameFilters) { _nameFilters = nameFilters; }

private:
    FileIndexDao _dao;
    QList<QDir> _directories;
    QStringList _nameFilters;

    qint32 indexDirectory(const QDir& dir);

    bool indexFile(const QFileInfo& fileInfo, const QDir& dir);
};

#endif // FILEINDEXER_H
