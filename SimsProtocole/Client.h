#ifndef CLIENT_H
#define CLIENT_H


#include <QTcpSocket>
#include <QString>
#include <QList>
#include <QFile>

#include "utils.h"




class Client : public QObject
{
    Q_OBJECT
public:
    Client(QTcpSocket *socket);
    Client(QString address);
    ~Client();
    QString getAddress();
    void sendMessage();
private:
    void socketConfig();

    void receivedFileRequest();     // demande de fichier de la part du client
    void receivedFileRequestInit(); // réponse du serveur lors de la demande (confirmation nom, taille)
    void receivedFileRequestAck();  // confirmation du client
    void receivedFileData();        // réception d'un fichier par le client
    void receivedFileList();

signals:
    void NewData(int);
    void connected(Client *);
    void disconnected();

private slots:

    void donneesRecues();
    void donneesEcrites(qint64);
    void deconnexionSocket();
    void connexion();

private:
    QTcpSocket *socket;
    quint16 messageLength;

    quint64 bytesReceived;
    quint64 filesize;
    QFile *fichierRecv;

    QFile *fichierSend;
    quint64 bytesSent;

    ETAT_CLIENT etat;

};

#endif // CLIENT_H
