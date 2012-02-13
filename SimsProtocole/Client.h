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
    void sendMessage(QString message);
signals:
    void NewData(int);
    void disconnected();

private slots:

    void donneesRecues();
    void donneesEcrites(qint64);
    void deconnexionSocket();
    void connexion();

private:
    QTcpSocket *socket;
    quint16 messageLength;

    QString currentFile;
    quint64 bytesReceived;
    quint64 filesize;
    QFile *fichierRecv;
    QFile *fichierSend;

    ETAT_CLIENT etat;

};

#endif // CLIENT_H
