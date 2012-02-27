#include "fileindexer.h"
#include <QDebug>
#include <QDirIterator>

FileIndexer::FileIndexer() : _dao(QSqlDatabase::database())
{
}

FileIndexer::FileIndexer(QSqlDatabase db) : _dao(db)
{
}

bool FileIndexer::addDirectory(QDir dir)
{
    if (_directories.contains(dir))
        return false;

        _directories << dir;
    return true;
}

bool FileIndexer::addDirectory(const QString& path)
{
    return addDirectory(QDir(path));
}

qint32 FileIndexer::updateDatabase()
{
    qint32 count = 0;
    qDebug() << "Indexing " << _directories.size() << " directories";
    foreach (QDir dir, _directories) {
        count += indexDirectory(dir);
    }
    return count;
}

QList<FileModel> FileIndexer::searchFiles(const QString& keyword)
{
    return _dao.searchFiles(keyword);
}

/*!
  * Index all files in dir and its subdirectories
  */
qint32 FileIndexer::indexDirectory(const QDir& dir)
{
    qint32 count = 0;
    QDirIterator it(dir.absolutePath(), _nameFilters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo info = it.fileInfo();
        //Add the file to the index
        if (indexFile(info, dir))
            count++;
    }
    return count;
}

QList<FileModel> FileIndexer::getAllIndexedFiles()
{
    return _dao.getAllFiles();
}

bool FileIndexer::indexFile(const QFileInfo& fileInfo, const QDir& dir)
{
    qDebug() << "Indexing file " << fileInfo.absoluteFilePath();
    FileModel model(fileInfo.fileName(), dir.absolutePath(), fileInfo.absoluteDir().absolutePath(), fileInfo.absoluteFilePath());
    return _dao.insertFile(model);
}
