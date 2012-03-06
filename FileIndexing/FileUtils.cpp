#include "FileUtils.h"
#include <QByteArray>
#include <QCryptographicHash>
#include <QFileInfo>
#include <QDebug>
FileUtils::FileUtils()
{
}

QString FileUtils::fileMd5Hash(QString path)
{
    QByteArray block(BLOCK_SIZE, 0);
    QCryptographicHash hash(QCryptographicHash::Md5);
    QFile file(path);
    file.open(QFile::ReadOnly);
    block = file.read(BLOCK_SIZE);
    while (!block.isEmpty()) {
        hash.addData(block);
        block = file.read(BLOCK_SIZE);
    }
    return QString(hash.result().toHex());
}
