#ifndef FOLDERMODEL_H
#define FOLDERMODEL_H

#include <QObject>
#include <QString>

class FolderModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id WRITE setId)
    Q_PROPERTY(QString path READ path WRITE setPath)

public:
    FolderModel(QObject * parent = 0);
    ~FolderModel();

    void setPath(const QString& path) { _path = path; }

    const QString& path() const { return _path; }

    void setId(int id) { _id = id; }

    int id() const { return _id; }

    QString toString() const;

private:
    int _id;
    QString _path;
};

QDebug operator<<(QDebug dbg, const FolderModel& model);

#endif // FOLDERMODEL_H
