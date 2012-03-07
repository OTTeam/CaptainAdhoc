#include "FileModel.h"
#include <sstream>
#include <QDebug>

FileModel::FileModel() : _indexTime(QDateTime::currentDateTime())
{
}

FileModel::FileModel(const QString& name, const QString& baseDir, const QString& dirPath, const QString& path, int size, const QString& hash) :
        _name(name), _baseDir(baseDir), _dirPath(dirPath), _path(path), _indexTime(QDateTime::currentDateTime()), _size(size), _hash(hash)
{
    _type = _name.section('.', -1);
}

QString FileModel::toString() const
{
    QString str = QString("FileModel { id = %1, name = %2, type = %3, baseDir = %4, dirPath = %5, path = %6, indexTime = %7, size = %8 }").
                  arg(_id).arg(_name).arg(_type).arg(_baseDir).arg(_dirPath).arg(_path).arg(_indexTime.toString()).arg(_size);
    return str;
}

SimpleFileModel FileModel::toSimpleFileModel() const
{
    return SimpleFileModel(_id, _name, _type, _size);
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
