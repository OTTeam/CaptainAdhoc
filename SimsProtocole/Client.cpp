#include <QHostAddress>

#include "utils.h"
#include "Client.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

Client::Client(QTcpSocket *s)
{
    socket = s;
    socketConfig();
}


Client::Client(QString address)
{
    socket = new QTcpSocket(this);
    socketConfig();
    socket->connectToHost(address,PORT_SERVEUR);
}


void Client::socketConfig()
{
    connect(socket, SIGNAL(connected()),this,SLOT(connexion()));
    connect(socket, SIGNAL(readyRead()),this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(deconnexionSocket()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this,SLOT(donneesEcrites(qint64)));
    messageLength = 0;
    etat = IDLE;
}


Client::~Client()
{
    if (fichierRecv != NULL && fichierRecv->isOpen())
    {
        fichierRecv->close();
        delete fichierRecv;
     }
    if (fichierSend!= NULL && fichierSend->isOpen())
    {
        fichierSend->close();
        delete fichierSend;
     }

    delete socket;
}

/*
 * Slot appelé à chaque fois que la socket reçoit des données.
 * Première étape : vérifier le nombre d'octets devant être lus.
 * Si on le connait déjà et que tout est arrivé, on lit le message
 *
 * Un message commence par un type (FILE_REQUEST_INIT, FILE_REQUEST, LIST_REQUEST)
 * FILE_REQUEST_INIT : 2 données : nom du fichier à recevoir, taille
 * FILE_REQUEST      : le fichier à recevoir (découpé), data brut.
 * LIST_REQUEST      : anuaire de fichiers disponibles : sous la forme nomFic1, taille1, nomFic2, Taille2....
 *
*/
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
                   QString currentFile;

                   in >> currentFile;
                   in >> filesize;

                   QString path = QFileDialog::getExistingDirectory(0,"Enregistrer le fichier sous...");
                   //QString path = "C:\\";
                   path += currentFile;

                   QMessageBox::information(0,"receiving", path);

                   fichierRecv = new QFile(path);
                   fichierRecv->open(QIODevice::WriteOnly | QIODevice::Truncate);

                   bytesReceived = 0;

                   QByteArray paquet;
                   QDataStream out(&paquet, QIODevice::WriteOnly);

                    // On prépare le paquet à envoyer
                    quint16 type = FILE_REQUEST_ACK;
                    out << (quint16) 0;    // taillePaquet que l'on changera après écriture du paquet
                    out << type;           // typePaquet

                    // mise à jour de taillePaquet
                    out.device()->seek(0);
                    out << (quint16) (paquet.size() - sizeof(quint16));

                    socket->write(paquet); // On envoie le paquet
           }
           else if (type == FILE_REQUEST)
           {
               QByteArray data;
               in >> data;
               bytesReceived += data.length();
               fichierRecv->write(data);
               emit NewData(bytesReceived*100/filesize);
              // QMessageBox::information(0,"receiving Data", QString::number(data.length()));
               if (bytesReceived == filesize)
               {
                   bytesReceived = 0;
                   fichierRecv->close();
                   delete fichierRecv;

               }
           }
           else if (type == FILE_REQUEST_ACK)
           {
               etat = SENDING_FILE;
               donneesEcrites(0);
           }
           else if (type == LIST_REQUEST)
           {
                //demande d'annuaire


           }


           messageLength = 0;
    }

}

/*
 * Fonction d'envoi de message, on lui passe en paramètre le fichier à envoyer et elle se charge de
 * le découper et de le passer par la socket. (envoi au préalable d'un message : FILE_REQUEST_INIT pour
 * annoncer la venue du fichier
*/
void Client::sendMessage(QString message)
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

     // On prépare le paquet à envoyer
     quint16 type = FILE_REQUEST_INIT;

     // le fichier à envoyer, uniquement à des fins de test
     QString filePath = QFileDialog::getOpenFileName(0,"Sélectionnez le fichier à envoyer");

     // ouverture du fichier
     fichierSend = new QFile(filePath);
     fichierSend->open(QIODevice::ReadOnly);
    // changement d'état pour pouvoir envoyer la suite lors de l'appel au slot "donneesEcrites"
    etat = WAITING_ACK;
    bytesSent=0;

    QFileInfo fileInfo(filePath);
    QString SendFilename = fileInfo.fileName();
    quint64 SendFilesize = fileInfo.size();
     out << (quint16) 0;    // taillePaquet que l'on changera après écriture du paquet
     out << type;           // typePaquet
     out << SendFilename;   // NomFichier
     out << SendFilesize;   //TailleFichier


     // mise à jour de taillePaquet
     out.device()->seek(0);
     out << (quint16) (paquet.size() - sizeof(quint16));


     QMessageBox::information(0,"sending", SendFilename);
     socket->write(paquet); // On envoie le paquet

}

/*
 * Slot appelé à chaque écriture de données sur la socket, permet de ne pas être bloquant sur le socket->write()
 * si le buffer est plein.
 */
void Client::donneesEcrites(qint64 bytes)
{
    if (etat == SENDING_FILE)
    {
        QFileInfo fileInfo(fichierSend->fileName());

        int filesize = fileInfo.size();
        quint16 type = FILE_REQUEST;
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        QByteArray data = fichierSend->read(BLOCK_SIZE);
        bytesSent += data.size();
        emit NewData(bytesSent*100/filesize);
        if (data.length() > 0)
        {
            out << (quint16) 0;
            out << type;
            out << data;

            out.device()->seek(0);
            out << (quint16) (paquet.size() - sizeof(quint16));

            socket->write(paquet);

        } else
        {
            fichierSend->close();
            delete fichierSend;
            bytesSent = 0;
            etat = IDLE;
        }
    }
}

void Client::deconnexionSocket()
{
     emit disconnected();
}

 void Client::connexion()
 {
     emit connected(this);
 }


QString Client::getAddress()
{
    return socket->localAddress().toString();
}
