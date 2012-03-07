#ifndef FILEINDEXER_H
#define FILEINDEXER_H

#include <QDir>
#include <QList>
#include "FileIndexDao.h"
#include "FolderDao.h"


class FileIndexer
{
public:
    FileIndexer(bool computeHash = false);
    FileIndexer(QSqlDatabase db, bool computeHash = false);

    bool addDirectory(QDir dir);
    bool addDirectory(const QString& path);

    qint32 updateDatabase();

    QList<FileModel> getAllIndexedFiles();

    QList<SimpleFileModel> getSharedFiles();

    QList<FileModel> searchFiles(QString keyword);

    void setDatabase(QSqlDatabase db) { _dao.setDatabase(db); _folderDao.setDatabase(db); }

    void setNameFilters(const QStringList& nameFilters) { _nameFilters = nameFilters; }

    bool isComputeHash() const { return _computeHash; }
    void setComputeHash(bool computeHash) { _computeHash = computeHash; }

private:
    FileIndexDao _dao;
    FolderDao _folderDao;
    QList<QDir> _directories;
    QStringList _nameFilters;
    bool _computeHash;

    qint32 indexDirectory(const QDir& dir);

    bool indexFile(const QFileInfo& fileInfo, const QDir& dir);
};

#endif // FILEINDEXER_H
