#include "SimpleFileModel.h"

SimpleFileModel::SimpleFileModel()
{
}

SimpleFileModel::SimpleFileModel(int id, const QString& name, const QString& type, int size) :
        _id(id), _name(name), _type(type), _size(size)
{
}

QString SimpleFileModel::toString() const
{
    QString str = QString("FileModel { id = %1, name = %2, type = %3, size = %4 }").
                  arg(_id).arg(_name).arg(_type).arg(_size);
    return str;
}

QDataStream& SimpleFileModel::readFromStream(QDataStream& in)
{
    QByteArray byteArray;

    in >> _id;
    in >> byteArray;
    _name = QString(byteArray);
    in >> byteArray;
    _type = QString(byteArray);
    in >> _size;
    return in;
}

QDebug operator<<(QDebug dbg, const SimpleFileModel& model)
{
    dbg.nospace() << model.toString();

    return dbg.space();
}

std::ostream& operator<<(std::ostream& out, const SimpleFileModel& model)
{
    out << model.toString().toStdString();
    return out;
}

QDataStream &operator<<(QDataStream& out, const SimpleFileModel& model)
{
    out << (quint32) model.id() << model.name().toAscii() << model.type().toAscii() << (quint32) model.size();
    return out;
}

QDataStream &operator>>(QDataStream& in, SimpleFileModel& model)
{
    return model.readFromStream(in);
}
