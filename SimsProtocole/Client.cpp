#include <QHostAddress>

#include "utils.h"
#include "Client.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTime>


/*
 * Constructeur appelé lorsque les serveur TCP a reçu
 * une nouvelle demande de connexion client
 */
Client::Client(QTcpSocket *s)
{
    _socket = s;
    // dans ce cas là, comme la connexion a été demandée par le socket
    // on sait que c'est à 1 hop
    _dest = socket()->peerAddress();
    _nextHop = socket()->peerAddress();
    _hopNumber = 1;

    ConfigClient();
    emit Connected();
}

/*
 * Constructeur appelé lors de l'ajout d'un client
 */
Client::Client(QTcpSocket *s, QHostAddress dest, quint8 hopNumber)
{
    _socket = s;
    _dest = dest;
    _hopNumber = hopNumber;

    ConfigClient();
}


/*
 * Constructeur appelé lors de l'ajout d'un client par son adresse.
 */
Client::Client(QHostAddress address)
{
    _socket = new QTcpSocket(this);
    ConfigClient();
    _nextHop = address;
    //_socket->connectToHost(address,PORT_SERVEUR);
}


void Client::ConfigClient()
{
    connect(_socket, SIGNAL(Connected()),         this, SIGNAL(Connected()));
    connect(_socket, SIGNAL(Disconnected()),      this, SIGNAL(Disconnected()));
    connect(_socket, SIGNAL(error(QAbstractSocket::SocketError)),this,SIGNAL(Disconnected()));
    _messageLength = 0;
    _etat = IDLE;


    _bytesReceived = 0;
    _previousBytesReceived = 0;
    _bytesSent = 0;

    _timerDlSpeed = new QTimer(this);
    connect(_timerDlSpeed,SIGNAL(timeout()),this,SLOT(dlSpeedMeasure()));
    _timerDlSpeed->setInterval(500);
    //timerDlSpeed->start();
    _timerDlSpeed->setSingleShot(false);

    _timerUlSpeed = new QTimer(this);
    connect(_timerUlSpeed,SIGNAL(timeout()),this,SLOT(ulSpeedMeasure()));
    _timerUlSpeed->setInterval(500);
    //timerUlSpeed->start();
    _timerUlSpeed->setSingleShot(false);
}


Client::~Client()
{
    if (_fileToReceive != NULL && _fileToReceive->isOpen())
    {
        _fileToReceive->close();
        delete _fileToReceive;
    }
    if (_fileToSend!= NULL && _fileToSend->isOpen())
    {
        _fileToSend->close();
        delete _fileToSend;
    }
    // on détruit le socket seulement si on est le next hop (pas si c'est une passerelle)
    if (_dest == _nextHop)
        delete _socket;
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
    QDataStream in(_socket);


    while (_socket->bytesAvailable()>0)
    {
        if (_messageLength == 0) // Si on ne connaît pas encore la taille du message, on essaie de la récupérer
        {
            if (_socket->bytesAvailable() < (int)sizeof(quint16)) // On n'a pas reçu la taille du message en entier
                return;

             in >> _messageLength; // Si on a reçu la taille du message en entier, on la récupère

        }

        // Si on connaît la taille du message, on vérifie si on a reçu le message en entier
        if (_socket->bytesAvailable() < _messageLength) // Si on n'a pas encore tout reçu, on arrête la méthode
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

        _messageLength = 0;
    }

}

void Client::receivedFileRequest()
{

}


void Client::receivedFileRequestInit()
{
    QDataStream in(_socket);
    // ici on a juste envoyé le filename, on le récupère donc
    QString fileRequested;

    in >> fileRequested;
    in >> _filesize;

    QString path = QFileDialog::getExistingDirectory(0,"Enregistrer le fichier sous...");
    path += "\\" + fileRequested;

    _fileToReceive = new QFile(path);
    _fileToReceive->open(QIODevice::WriteOnly | QIODevice::Truncate);

    _bytesReceived = 0;


    // on envoie alors le Ack pour confirmer au serveur l'envoi du fichier
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer
    quint16 type = FILE_REQUEST_ACK;
    out << (quint16) 0;    // taillePaquet globale que l'on changera après écriture du paquet
    out << _dest.toString();    //la destination du paquet
    out << _socket->localAddress().toString(); // l'expéditeur du paquet (nous même)
    out << (quint16) sizeof(type); // taille du data, ici c'est juste type, du coup pas de traitement
    out << type;           // typePaquet

    // mise à jour de taillePaquet
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));

    _socket->write(paquet); // On envoie le paquet
    _timerDlSpeed->start();
}


void Client::receivedFileRequestAck()
{
    _etat = SENDING_FILE;
    _timerUlSpeed->start();
    newBytesWritten(0);
}


void Client::receivedFileData()
{
    QDataStream in(_socket);
    QByteArray data;
    in >> data;
    _bytesReceived += data.length();
    _fileToReceive->write(data);
    emit BytesReceivedUpdate(_bytesReceived*100/_filesize);

    if (_bytesReceived == _filesize)
    {
        _bytesReceived = 0;
        _fileToReceive->close();
        delete _fileToReceive;
        _timerDlSpeed->stop();
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
    _fileToSend = new QFile(filePath);
    _fileToSend->open(QIODevice::ReadOnly);

    // changement d'état pour pouvoir envoyer la suite lors de l'appel au slot "donneesEcrites"
    _etat = WAITING_ACK;
    _bytesSent=0;



    QFileInfo fileInfo(filePath);
    QString SendFilename = fileInfo.fileName();
    quint64 SendFilesize = fileInfo.size();
    quint64 posData;
    quint16 headerSize;

    out << (quint16) 0;
    out << _dest.toString();
    out << _socket->localAddress().toString();
    posData = out.device()->pos();

    out << (quint16) 0;    // taillePaquet que l'on changera après écriture du paquet
    headerSize = paquet.size();

    out << type;           // typePaquet
    out << SendFilename;   // NomFichier
    out << SendFilesize;   //TailleFichier


    // mise à jour de taillePaquet globale
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));


    // puis de la taille du paquet data (sans le header
    out.device()->seek(posData);
    out << (quint16) (paquet.size() - headerSize);

    QMessageBox::information(0,"sending", SendFilename);
    _socket->write(paquet); // On envoie le paquet
}

void Client::ForwardMessage(QHostAddress senderAdd,QHostAddress destAdd)
{
    // On commence par lire le paquet, sans l'interpréter.
    // On renvoie alors le paquet complet à notre next-hop
    QDataStream in(_socket);
    quint16 dataSize;
    QByteArray data;
    in >> dataSize;
    data.resize(dataSize);
    in.readRawData(data.data(),dataSize);


    QByteArray paquetData;
    QDataStream out(&paquetData,QIODevice::WriteOnly );

    out << (quint16) 0;
    out << senderAdd.toString();
    out << destAdd.toString();
    out << dataSize;
    out << data;

    out.device()->seek(0);
    out << (quint16) (paquetData.size() - sizeof(quint16));

    _socket->write(paquetData); // On envoie le paquet
}


/*
 * Slot appelé à chaque écriture de données sur la socket, permet de ne pas être bloquant sur le socket->write()
 * si le buffer est plein.
 */
void Client::newBytesWritten(qint64 bytes)
{
    Q_UNUSED(bytes)

    if (_etat == SENDING_FILE)
    {
        QFileInfo fileInfo(_fileToSend->fileName());

        int filesize = fileInfo.size();
        quint16 type = FILE_DATA;
        QByteArray paquet;
        QDataStream out(&paquet, QIODevice::WriteOnly);

        QByteArray data = _fileToSend->read(BLOCK_SIZE);
        _bytesSent += data.size();
        emit BytesSentUpdate(_bytesSent*100/filesize);
        if (data.length() > 0)
        {
            out << (quint16) 0;
            out << _dest.toString();
            out << _socket->localAddress().toString();
            out << (quint16) (sizeof(type) + data.size());
            out << type;
            out << data;

            out.device()->seek(0);
            out << (quint16) (paquet.size() - sizeof(quint16));

            _socket->write(paquet);

        } else
        {
            _fileToSend->close();
            delete _fileToSend;
            _bytesSent = 0;
            _timerUlSpeed->stop();
            _etat = IDLE;
        }
    }
}


QHostAddress Client::address()
{
    return _socket->peerAddress();
}

quint8 Client::hopNumber()
{
    return _hopNumber;
}

QTcpSocket *Client::socket()
{
    return _socket;
}

void Client::UpdateRoute(QTcpSocket *s,quint8 newHopNumber)
{
    _socket = s;
    _hopNumber = newHopNumber;
}

void Client::connectSocket()
{
    _socket->connectToHost(_nextHop,PORT_SERVEUR);
}

void Client::dlSpeedMeasure()
{
    quint64 bytesDiff = (_bytesReceived-_previousBytesReceived)*2;
    _previousBytesReceived = _bytesReceived;

    emit DownloadSpeedUpdate(bytesDiff);
}

void Client::ulSpeedMeasure()
{
    quint64 bytesDiff = (_bytesSent-_previousBytesSent)*2;
    _previousBytesSent = _bytesSent;

    emit UploadSpeedUpdate(bytesDiff);
}
