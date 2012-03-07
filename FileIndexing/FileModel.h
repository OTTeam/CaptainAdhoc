#ifndef FILEMODEL_H
#define FILEMODEL_H

#include <QString>
#include <QDateTime>
#include <iostream>
#include "SimpleFileModel.h"

class FileModel
{
public:
    FileModel();
    FileModel(const QString& name, const QString& baseDir, const QString& dirPath, const QString& path, int size, const QString& hash);

    int id() const { return _id; }
    void setId(int id) { _id = id; }

    const QString& name() const { return _name; }
    void setName(const QString& name) { _name = name; }

    const QString& type() const { return _type; }
    void setType(const QString& type) { _type = type; }

    const QString& baseDir() const { return _baseDir; }
    void setBaseDir(const QString& baseDir) { _baseDir = baseDir; }

    const QString& path() const { return _path; }
    void setPath(const QString& path) { _path = path; }

    const QString& dirPath() const { return _dirPath; }
    void setDirPath(const QString& dirPath) { _dirPath = dirPath; }

    const QDateTime& indexTime() const { return _indexTime; }
    void setIndexTime(const QDateTime& indexTime) { _indexTime = indexTime; }

    int size() const { return _size; }
    void setSize(int size) { _size = size ; }

    const QString& hash() const { return _hash; }
    void setHash(const QString& hash) { _hash = hash; }

    SimpleFileModel toSimpleFileModel() const;

    QString toString() const;

private:
    int _id;
    QString _name;
    QString _type;
    QString _baseDir;
    QString _dirPath;
    QString _path;
    QDateTime _indexTime;
    int _size;
    QString _hash;
};

std::ostream& operator<<(std::ostream& out, const FileModel& model);

QDebug operator<<(QDebug dbg, const FileModel& model);

#endif // FILEMODEL_H
