#ifndef FILEUTILS_H
#define FILEUTILS_H
#include <QFile>

const int BLOCK_SIZE = 4096 * 2;

class FileUtils
{
public:
    FileUtils();

    static QString fileMd5Hash(QString path);
};

#endif // FILEUTILS_H
