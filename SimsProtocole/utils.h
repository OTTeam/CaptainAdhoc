#ifndef UTILS_H
#define UTILS_H


#include <QString>

class Client;

#define PORT_SERVEUR 50855
#define DISCOVERY_PORT 50854

#define BROADCAST_INTERVAL 10000
#define BLOCK_SIZE 50000


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


enum TYPE_PAQUET { FILE_REQUEST, FILE_REQUEST_INIT, FILE_DATA, FILE_REQUEST_ACK, LIST_REQUEST };


enum ETAT_CLIENT { IDLE, SENDING_FILE, WAITING_ACK};
#endif // UTILS_H
