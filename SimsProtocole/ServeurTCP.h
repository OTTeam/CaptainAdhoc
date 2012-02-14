#ifndef SERVEURTCP_H
#define SERVEURTCP_H

#include <QObject>
#include <QTcpServer>


class ServeurTCP : public QObject
{
    Q_OBJECT
public:
    explicit ServeurTCP(QObject *parent = 0);
    bool ServerStarted();
    bool StartServer();

signals:
   void AjouterClient(QTcpSocket *);

private slots:
    void nouvelleConnexion();

private:
    QTcpServer *serveur;

    bool serverStarted;

};

#endif // SERVEURTCP_H
