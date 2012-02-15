#include "ClientDiscovery.h"
#include "utils.h"

ClientDiscovery::ClientDiscovery(QObject *parent) :
    QObject(parent)
{

    socket = new QUdpSocket(this);
    socket->bind(DISCOVERY_PORT,QUdpSocket::ShareAddress);
    connect(socket,SIGNAL(readyRead()),this,SLOT(newDatagramAvailable()));

    timer = new QTimer(this);

    timer->setInterval(BROADCAST_INTERVAL);
    timer->setSingleShot(false);
    connect(timer,SIGNAL(timeout()),this,SLOT(sendNewDatagram()));
    timer->start();
}


void ClientDiscovery::newDatagramAvailable()
{
    //on vient de recevoir un nouveau datagram, on vérifie
    //que c'est bien un datagram de notre programme
    // puis on émet le signal d'ajout de client si nécessaire
    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        QHostAddress senderAddress;
        datagram.resize(socket->pendingDatagramSize());
        socket->readDatagram(datagram.data(), datagram.size(),&senderAddress);
        QString text = datagram.data();

        qDebug()<< "datagram reçu : " << text;
        if (text == "CaptainAdHocBroadCast" && senderAddress != socket->localAddress())
        {
            emit DatagramReceived(senderAddress.toString());
        }
    }

}


void ClientDiscovery::sendNewDatagram()
{
    QByteArray datagram = "CaptainAdHocBroadCast";

    socket->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, DISCOVERY_PORT);
    qDebug()<< "datagram envoyé : " << datagram.data();
}
