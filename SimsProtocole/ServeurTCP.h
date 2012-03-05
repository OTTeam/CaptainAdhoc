#ifndef SERVEURTCP_H
#define SERVEURTCP_H

#include <QObject>
#include <QTcpServer>


class TCPServer : public QObject
{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent = 0);
    bool ServerStarted();
    bool StartServer();

signals:
   void AjouterClient(QTcpSocket *);

private slots:
    void nouvelleConnexion();

private:
    QTcpServer *_serveur;

    bool _serverStarted;

};

#endif // SERVEURTCP_H
