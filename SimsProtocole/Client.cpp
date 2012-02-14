#include <QHostAddress>

#include "utils.h"
#include "Client.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

Client::Client(QTcpSocket *s)
{
    socket = s;
    socketConfig();}


Client::Client(QString address)
{
    socket = new QTcpSocket(this);
    socketConfig();
    socket->connectToHost(address,PORT_SERVEUR);
}


void Client::socketConfig()
{
    connect(socket, SIGNAL(connected()),         this, SLOT(connexion()));
    connect(socket, SIGNAL(readyRead()),         this, SLOT(donneesRecues()));
    connect(socket, SIGNAL(disconnected()),      this, SLOT(deconnexionSocket()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(donneesEcrites(qint64)));
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
 * Slot appel� � chaque fois que la socket re�oit des donn�es.
 * Premi�re �tape : v�rifier le nombre d'octets devant �tre lus.
 * Si on le connait d�j� et que tout est arriv�, on lit le message
 *
 * Un message commence par un type (FILE_REQUEST_INIT, FILE_REQUEST, LIST_REQUEST)
 * FILE_REQUEST_INIT : 2 donn�es : nom du fichier � recevoir, taille
 * FILE_REQUEST      : le fichier � recevoir (d�coup�), data brut.
 * LIST_REQUEST      : anuaire de fichiers disponibles : sous la forme nomFic1, taille1, nomFic2, Taille2....
 *
*/
void Client::donneesRecues()
{
    // Si tout va bien, on continue : on r�cup�re le message
    QDataStream in(socket);

    while (socket->bytesAvailable()>0)
    {
        if (messageLength == 0) // Si on ne conna�t pas encore la taille du message, on essaie de la r�cup�rer
        {
            if (socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas re�u la taille du message en entier
                return;

            in >> messageLength; // Si on a re�u la taille du message en entier, on la r�cup�re
        }

        // Si on conna�t la taille du message, on v�rifie si on a re�u le message en entier
        if (socket->bytesAvailable() < messageLength) // Si on n'a pas encore tout re�u, on arr�te la m�thode
            return;

        // Si ces lignes s'ex�cutent, c'est qu'on a re�u tout le message : on peut le r�cup�rer !
        quint16 type;
        in >> type;
        switch (type)
        {
        case FILE_REQUEST_INIT:
            receivedFileRequestInit();
            break;
        case FILE_DATA:
            receivedFileData();
            break;
        case FILE_REQUEST_ACK:
            receivedFileRequestAck();
            break;
        case LIST_REQUEST:
            receivedFileList();
            break;
        default:
            break;
        }

        messageLength = 0;
    }

}



void Client::receivedFileRequest()
{

}


void Client::receivedFileRequestInit()
{
    QDataStream in(socket);
    // ici on a juste envoy� le filename, on le r�cup�re donc
    QString fileRequested;

    in >> fileRequested;
    in >> filesize;

    QString path = QFileDialog::getExistingDirectory(0,"Enregistrer le fichier sous...");
    path += "\\" + fileRequested;

    QMessageBox::information(0,"receiving", path);

    fichierRecv = new QFile(path);
    fichierRecv->open(QIODevice::WriteOnly | QIODevice::Truncate);

    bytesReceived = 0;


    // on envoie alors le Ack pour confirmer au serveur l'envoi du fichier
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On pr�pare le paquet � envoyer
    quint16 type = FILE_REQUEST_ACK;
    out << (quint16) 0;    // taillePaquet que l'on changera apr�s �criture du paquet
    out << type;           // typePaquet

    // mise � jour de taillePaquet
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    socket->write(paquet); // On envoie le paquet
}


void Client::receivedFileRequestAck()
{
    etat = SENDING_FILE;
    donneesEcrites(0);
}


void Client::receivedFileData()
{
    QDataStream in(socket);
    QByteArray data;
    in >> data;
    bytesReceived += data.length();
    fichierRecv->write(data);
    emit NewData(bytesReceived*100/filesize);

    if (bytesReceived == filesize)
    {
        bytesReceived = 0;
        fichierRecv->close();
        delete fichierRecv;

    }
}

void Client::receivedFileList()
{

}


/*
 * Fonction d'envoi de message, on lui passe en param�tre le fichier � envoyer et elle se charge de
 * le d�couper et de le passer par la socket. (envoi au pr�alable d'un message : FILE_REQUEST_INIT pour
 * annoncer la venue du fichier
*/
void Client::sendMessage()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On pr�pare le paquet � envoyer
    quint16 type = FILE_REQUEST_INIT;

    // le fichier � envoyer, uniquement � des fins de test
    QString filePath = QFileDialog::getOpenFileName(0,"S�lectionnez le fichier � envoyer");

    // ouverture du fichier
    fichierSend = new QFile(filePath);
    fichierSend->open(QIODevice::ReadOnly);
    // changement d'�tat pour pouvoir envoyer la suite lors de l'appel au slot "donneesEcrites"
    etat = WAITING_ACK;
    bytesSent=0;

    QFileInfo fileInfo(filePath);
    QString SendFilename = fileInfo.fileName();
    quint64 SendFilesize = fileInfo.size();
    out << (quint16) 0;    // taillePaquet que l'on changera apr�s �criture du paquet
    out << type;           // typePaquet
    out << SendFilename;   // NomFichier
    out << SendFilesize;   //TailleFichier


    // mise � jour de taillePaquet
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));


    QMessageBox::information(0,"sending", SendFilename);
    socket->write(paquet); // On envoie le paquet

}

/*
 * Slot appel� � chaque �criture de donn�es sur la socket, permet de ne pas �tre bloquant sur le socket->write()
 * si le buffer est plein.
 */
void Client::donneesEcrites(qint64 bytes)
{
    if (etat == SENDING_FILE)
    {
        QFileInfo fileInfo(fichierSend->fileName());

        int filesize = fileInfo.size();
        quint16 type = FILE_DATA;
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
