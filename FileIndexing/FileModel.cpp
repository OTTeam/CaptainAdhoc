#include "FileModel.h"
#include <sstream>
#include <QDebug>

FileModel::FileModel() : _indexTime(QDateTime::currentDateTime())
{
}

FileModel::FileModel(const QString& name, const QString& baseDir, const QString& dirPath, const QString& path, int size) :
        _name(name), _baseDir(baseDir), _dirPath(dirPath), _path(path), _indexTime(QDateTime::currentDateTime()), _size(size)
{
}

QString FileModel::toString() const
{
    QString str = QString("FileModel { id = %1, name = %2, baseDir = %3, dirPath = %4, path = %5, indexTime = %6, size = %7 }").
                  arg(_id).arg(_name).arg(_baseDir).arg(_dirPath).arg(_path).arg(_indexTime.toString()).arg(_size);
    return str;
}

QDebug operator<<(QDebug dbg, const FileModel& model)
{
    dbg.nospace() << model.toString();

    return dbg.space();
}

std::ostream& operator<<(std::ostream& out, const FileModel& model)
{
    out << model.toString().toStdString();
    return out;
}
