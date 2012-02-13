#include <QHostAddress>

#include "utils.h"
#include "Client.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

Client::Client(QTcpSocket *s)
{
    socket = s;
    connect(socket,SIGNAL(readyRead()),this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnexionSocket()));
    connect(socket,SIGNAL(connected()),this,SLOT(connexion()));
    connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(donneesEcrites(qint64)));
    messageLength = 0;
    etat = IDLE;
}


Client::Client(QString address)
{
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()),this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnexionSocket()));
    connect(socket,SIGNAL(connected()),this,SLOT(connexion()));
    connect(socket,SIGNAL(bytesWritten(qint64)),this,SLOT(donneesEcrites(qint64)));
    messageLength = 0;

    socket->connectToHost(address,PORT_SERVEUR);

    etat = IDLE;
}

Client::~Client()
{
    delete socket;
}

void Client::donneesRecues()
{
    // Si tout va bien, on continue : on récupère le message
       QDataStream in(socket);

       while (socket->bytesAvailable()>0)
       {
           if (messageLength == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
           {
               if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
                    return;

               in >> messageLength; // Si on a reçu la taille du message en entier, on la récupère
           }

           // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
           if (socket->bytesAvailable() < messageLength) // Si on n'a pas encore tout reçu, on arrête la méthode
               return;

           // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
           quint16 type;
           in >> type;

           if (type == FILE_REQUEST_INIT)
           {
                   // ici on a juste envoyé le filename, on le récupère donc
                   in >> currentFile;
                   in >> filesize;

                   QString path = "C:\\";
                   path += currentFile;

                   //QMessageBox::information(0,"receiving", path);

                   fichierRecv = new QFile(path);
                   fichierRecv->open(QIODevice::WriteOnly | QIODevice::Truncate);

                   bytesReceived = 0;


           }
           else if (type == FILE_REQUEST)
           {
               QByteArray data;
               in >> data;
               bytesReceived += data.length();
               fichierRecv->write(data);
               emit NewData(bytesReceived/filesize*100);
              // QMessageBox::information(0,"receiving Data", QString::number(data.length()));
               if (bytesReceived == filesize)
               {
                   bytesReceived = 0;
                   fichierRecv->close();
                   delete fichierRecv;

               }
           }
           else if (type == LIST_REQUEST)
           {
                //demande d'annuaire


           }
           /*QString DataReceived;
           QString DataReceived2;
           in >> DataReceived;
           in >> DataReceived2;

           QMessageBox::information(0,"received1", DataReceived,QMessageBox::Ok);
           QMessageBox::information(0,"received2", DataReceived2,QMessageBox::Ok);



           emit NewData(DataReceived);*/

           messageLength = 0;
    }

}


void Client::sendMessage(QString message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

     // On prépare le paquet à envoyer

    // plusieurs types de paquets : demande de DL, demande d'annuaire
    // demande de DL sous la forme : TYPE_PAQUET | sizeFilename | filename
    // demande d'annuaire sous la forme : TYPE_PAQUET

     quint16 type = FILE_REQUEST_INIT;

     QString filePath = QFileDialog::getOpenFileName(0,"SElectionnez le fichier à envoyer");

     fichierSend = new QFile(filePath);
     fichierSend->open(QIODevice::ReadOnly);

    etat = SENDING;

    QFileInfo fileInfo(filePath);
    QString filename = fileInfo.fileName();

     out << (quint16) 0;    // taillePaquet
     out << type;           // typePaquet
     out << filename;       // NomFichier
     out << (quint64) fileInfo.size(); //TailleFichier

     out.device()->seek(0);
     out << (quint16) (paquet.size() - sizeof(quint16));

     QMessageBox::information(0,"sending", filename);
     socket->write(paquet); // On envoie le paquet




}


void Client::donneesEcrites(qint64 bytes)
{
    static int sentData = 0;
    int filesize = fichierSend->size();

    quint16 type = FILE_REQUEST;
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    QByteArray data = fichierSend->read(5000);
    if (data.length() > 0)
    {
        out << (quint16) 0;
        out << type;
        out << data;

        out.device()->seek(0);
        out << (quint16) (paquet.size() - sizeof(quint16));

        socket->write(paquet);
        sentData +=data.size();

        emit NewData(sentData/filesize*100);
    } else
    {
        fichierSend->close();
        delete fichierSend;
        sentData = 0;
    }
}

void Client::deconnexionSocket()
{

    QMessageBox::information(0, "Client", "Deconnexion client",QMessageBox::Ok);
    emit disconnected();

}

 void Client::connexion()
 {
     QMessageBox::information(0, "Client", "Connexion réussie",QMessageBox::Ok);
 }


QString Client::getAddress()
{
    return socket->localAddress().toString();
}
