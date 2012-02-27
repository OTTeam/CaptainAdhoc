#include <QHostAddress>

#include "utils.h"
#include "Client.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTime>

Client::Client(QTcpSocket *s)
{
    socket = s;
    ConfigClient();
}


Client::Client(QHostAddress address)
{
    socket = new QTcpSocket(this);
    ConfigClient();
    socket->connectToHost(address,PORT_SERVEUR);
}


void Client::ConfigClient()
{
    connect(socket, SIGNAL(connected()),         this, SLOT(socketConnection()));
    connect(socket, SIGNAL(readyRead()),         this, SLOT(newBytesReceived()));
    connect(socket, SIGNAL(disconnected()),      this, SLOT(socketDisconnection()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(newBytesWritten(qint64)));
    messageLength = 0;
    etat = IDLE;


    bytesReceived = 0;
    previousBytesReceived = 0;
    bytesSent = 0;

    timerDlSpeed = new QTimer(this);
    connect(timerDlSpeed,SIGNAL(timeout()),this,SLOT(dlSpeedMeasure()));
    timerDlSpeed->setInterval(500);
    //timerDlSpeed->start();
    timerDlSpeed->setSingleShot(false);

    timerUlSpeed = new QTimer(this);
    connect(timerUlSpeed,SIGNAL(timeout()),this,SLOT(ulSpeedMeasure()));
    timerUlSpeed->setInterval(500);
    //timerUlSpeed->start();
    timerUlSpeed->setSingleShot(false);
}


Client::~Client()
{
    if (fileToReceive != NULL && fileToReceive->isOpen())
    {
        fileToReceive->close();
        delete fileToReceive;
    }
    if (fileToSend!= NULL && fileToSend->isOpen())
    {
        fileToSend->close();
        delete fileToSend;
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
void Client::newBytesReceived()
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
    // ici on a juste envoyé le filename, on le récupère donc
    QString fileRequested;

    in >> fileRequested;
    in >> filesize;

    QString path = QFileDialog::getExistingDirectory(0,"Enregistrer le fichier sous...");
    path += "\\" + fileRequested;

    fileToReceive = new QFile(path);
    fileToReceive->open(QIODevice::WriteOnly | QIODevice::Truncate);

    bytesReceived = 0;


    // on envoie alors le Ack pour confirmer au serveur l'envoi du fichier
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
    timerDlSpeed->start();
}


void Client::receivedFileRequestAck()
{
    etat = SENDING_FILE;
    timerUlSpeed->start();
    newBytesWritten(0);
}


void Client::receivedFileData()
{
    QDataStream in(socket);
    QByteArray data;
    in >> data;
    bytesReceived += data.length();
    fileToReceive->write(data);
    emit BytesReceivedUpdate(bytesReceived*100/filesize);

    if (bytesReceived == filesize)
    {
        bytesReceived = 0;
        fileToReceive->close();
        delete fileToReceive;
        timerDlSpeed->stop();
    }
}

void Client::receivedFileList()
{

}


/*
 * Fonction d'envoi de message, on lui passe en paramètre le fichier à envoyer et elle se charge de
 * le découper et de le passer par la socket. (envoi au préalable d'un message : FILE_REQUEST_INIT pour
 * annoncer la venue du fichier
*/
void Client::sendMessage()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer
    quint16 type = FILE_REQUEST_INIT;

    // le fichier à envoyer, uniquement à des fins de test
    QString filePath = QFileDialog::getOpenFileName(0,"Sélectionnez le fichier à envoyer");

    // ouverture du fichier
    fileToSend = new QFile(filePath);
    fileToSend->open(QIODevice::ReadOnly);

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
void Client::newBytesWritten(qint64 bytes)
{
    if (etat == SENDING_FILE)
    {
        QFileInfo fileInfo(fileToSend->fileName());

        int filesize = fileInfo.size();
        quint16 type = FILE_DATA;
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        QByteArray data = fileToSend->read(BLOCK_SIZE);
        bytesSent += data.size();
        emit BytesSentUpdate(bytesSent*100/filesize);
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
            fileToSend->close();
            delete fileToSend;
            bytesSent = 0;
            timerUlSpeed->stop();
            etat = IDLE;
        }
    }
}


void Client::socketDisconnection()
{
    emit disconnected();
}


void Client::socketConnection()
{
    emit connected(this);
}


QHostAddress Client::getAddress()
{
    return socket->peerAddress();
}


void Client::dlSpeedMeasure()
{
    quint64 bytesDiff = (bytesReceived-previousBytesReceived)*2;
    previousBytesReceived = bytesReceived;

    emit DownloadSpeedUpdate(bytesDiff);
}

void Client::ulSpeedMeasure()
{
    quint64 bytesDiff = (bytesSent-previousBytesSent)*2;
    previousBytesSent = bytesSent;

    emit UploadSpeedUpdate(bytesDiff);
}
