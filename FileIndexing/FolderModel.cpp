#include "FolderModel.h"
#include <QDebug>

FolderModel::FolderModel(QObject * parent) : QObject(parent)
{
}

FolderModel::~FolderModel()
{
}

QString FolderModel::toString() const
{
    QString str = QString("FolderModel { id = %1, path = %2 }").arg(_id).arg(_path);;
    return str;
}

QDebug operator<<(QDebug dbg, const FolderModel& model)
{
    dbg.nospace() << model.toString();

    return dbg.space();
}
