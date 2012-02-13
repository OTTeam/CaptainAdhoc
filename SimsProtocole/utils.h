#ifndef UTILS_H
#define UTILS_H


#include <QString>

class Client;

#define PORT_SERVEUR 50855



struct LocalFiles
{
    QString path;
    QString filename;
    quint32 size;
};

struct RemoteFiles
{
    QString filename;
    quint32 size;
    Client *client;
};

enum TYPE_PAQUET { FILE_REQUEST_INIT, FILE_REQUEST, LIST_REQUEST };


enum ETAT_CLIENT { IDLE, SENDING};
#endif // UTILS_H
