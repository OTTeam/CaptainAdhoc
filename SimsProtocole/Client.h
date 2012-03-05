#ifndef CLIENT_H
#define CLIENT_H


#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QList>
#include <QFile>
#include <QTimer>
#include "utils.h"




class Client : public QObject
{
    Q_OBJECT
public:
    Client(QTcpSocket *socket);
    Client(QHostAddress address);
    Client(QTcpSocket *socket, QHostAddress dest, quint8 hopNumber);
    ~Client();
    QHostAddress address();
    quint8 hopNumber();
    QTcpSocket *socket();
    void UpdateRoute(QTcpSocket *,quint8);
    void connectSocket();
    void sendMessage();

    void ForwardMessage(QHostAddress senderAdd,QHostAddress destAdd);
private:
    void ConfigClient();

    void receivedFileRequest();     // demande de fichier de la part du client
    void receivedFileRequestInit(); // réponse du serveur lors de la demande (confirmation nom, taille)
    void receivedFileRequestAck();  // confirmation du client
    void receivedFileData();        // réception d'un fichier par le client
    void receivedFileList();

signals:
    void NewData(int);
    void BytesSentUpdate(int);
    void BytesReceivedUpdate(int);

    void DownloadSpeedUpdate(int);
    void UploadSpeedUpdate(int);

    void Connected();
    void Disconnected();

public slots:
    void newBytesReceived();
    void newBytesWritten(qint64);

private slots:
    void dlSpeedMeasure();
    void ulSpeedMeasure();

private:
    QTcpSocket *_socket;
    quint8 _hopNumber;

    QHostAddress _dest;
    QHostAddress _nextHop;

    quint16 _messageLength;

    quint64 _bytesReceived;
    quint64 _previousBytesReceived;

    quint64 _filesize;

    QFile *_fileToReceive;
    QFile *_fileToSend;

    quint64 _bytesSent;
    quint64 _previousBytesSent;

    ETAT_CLIENT _etat;

    QTimer *_timerDlSpeed;
    QTimer *_timerUlSpeed;

};

#endif // CLIENT_H
