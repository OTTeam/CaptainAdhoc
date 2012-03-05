#include <QHostInfo>

#include "ClientDiscovery.h"
#include "utils.h"

ClientDiscovery::ClientDiscovery(QObject *parent) :
    QObject(parent)
{

    _socket = new QUdpSocket(this);
    _socket->bind(DISCOVERY_PORT,QUdpSocket::ShareAddress);
    connect(_socket,SIGNAL(readyRead()),this,SLOT(newDatagramAvailable()));
}


void ClientDiscovery::newDatagramAvailable()
{
    //on vient de recevoir un nouveau datagram, on vérifie
    //que c'est bien un datagram de notre programme
    // puis on émet le signal d'ajout de client si nécessaire
    while (_socket->hasPendingDatagrams())
    {
        // récupération du datagram reçu
        QByteArray datagram;
        QHostAddress senderAddress;
        datagram.resize(_socket->pendingDatagramSize());
        _socket->readDatagram(datagram.data(), datagram.size(),&senderAddress);

        //QString text = datagram.data();

        // création d'un datastream pour lire plus simplement
        QDataStream in(datagram);

        qDebug()<< "BROADCAST RECEIVE" ;
        qDebug()<< "---------------------------------------" ;

        qDebug() << "----- senderAddress" << senderAddress.toString() << "-----";


        // on place les adresses reçues dans une liste
        QList<RoutesTableElt> routesReceived;
        while (!in.atEnd())
        {
            RoutesTableElt newElt;
            in >> newElt.destAddr;
            in >> newElt.hopNumber;

            // on incrémente le hop number car on a la passerelle en plus
            newElt.hopNumber++;

            routesReceived.push_back(newElt);
            qDebug() << "Address :" << newElt.destAddr.toString() << " -- Hop :" << newElt.hopNumber;

        }


        QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());
        bool localSent = false;
        qDebug() << "************* Local ***************";
        foreach (QHostAddress add, hostInfo.addresses())
        {
            qDebug() << add.toString();
            if (senderAddress == add)
            {
                localSent = true;
            }
        }
        if (senderAddress == _socket->localAddress())
            localSent = true;
        qDebug() << _socket->localAddress().toString();
        qDebug() << "***********************************";

        if (localSent == false)
            emit DatagramReceived(senderAddress,routesReceived);

    }

}


void ClientDiscovery::sendNewDatagram(QList<Client *> routesList )
{

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);

    // écriture du nombre de routes
    out << (quint16) routesList.size();
    // écriture de la liste des routes

    qDebug()<< "BROADCAST SEND" ;
    qDebug()<< "---------------------------------------" ;
    foreach(Client *client, routesList)
    {
        out << client->address().toString();
        out << client->hopNumber();
        qDebug() << "Address :" << client->address().toString() << " -- hop :" << client->hopNumber();
    }
    qDebug()<< "---------------------------------------" ;
    _socket->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, DISCOVERY_PORT);
    qDebug()<< "datagram envoyé : " << datagram.data();
}
