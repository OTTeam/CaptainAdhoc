#ifndef SIMPLEFILEMODEL_H
#define SIMPLEFILEMODEL_H

#include <QString>
#include <QDateTime>
#include <QDebug>
#include <iostream>

class SimpleFileModel
{
public:
    SimpleFileModel();
    SimpleFileModel(int id, const QString& name, const QString& type, int size);

    int id() const { return _id; }
    void setId(int id) { _id = id; }

    const QString& name() const { return _name; }
    void setName(const QString& name) { _name = name; }

    const QString& type() const { return _type; }
    void setType(const QString& type) { _type = type; }

    int size() const { return _size; }
    void setSize(int size) { _size = size ; }

    QString toString() const;

    QDataStream& readFromStream(QDataStream& in);

private:
    int _id;
    QString _name;
    QString _type;
    int _size;
};

std::ostream& operator<<(std::ostream& out, const SimpleFileModel& model);

QDebug operator<<(QDebug dbg, const SimpleFileModel& model);

QDataStream &operator<<(QDataStream &, const SimpleFileModel &);

QDataStream &operator>>(QDataStream &, SimpleFileModel &);

#endif // SIMPLEFILEMODEL_H
