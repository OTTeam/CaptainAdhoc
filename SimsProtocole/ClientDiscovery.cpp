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

//        qDebug()<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" ;
//        qDebug()<< "BROADCAST RECEIVE" ;


//        qDebug() << ">>>>> senderAddress" << senderAddress.toString() << ">>>>>>";

        qint16 RouteListSize = 0;
        in >> RouteListSize;
//        qDebug() << "RouteList size : " << RouteListSize;
        // on place les adresses reçues dans une liste
        QList<RoutesTableElt> routesReceived;
        routesReceived.clear();

        QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());

        for(int i = 0; i<RouteListSize; i++)
        {
            RoutesTableElt newElt;

            QString destAddrStr;
            in >> destAddrStr;
            in >> newElt.hopNumber;

            newElt.destAddr = destAddrStr;

            // on incrémente le hop number car on a la passerelle en plus
            newElt.hopNumber++;
            if (newElt.destAddr != _socket->localAddress() && !hostInfo.addresses().contains(newElt.destAddr))

            {     routesReceived.push_back(newElt);
//                qDebug() << "AddressStr :" << destAddrStr << "Address :" << newElt.destAddr << " -- Hop :" << newElt.hopNumber;
            }

        }


        bool localSent = false;
//        qDebug() << "************* Local ***************";
        foreach (QHostAddress add, hostInfo.addresses())
        {
//            qDebug() << add.toString();
            if (senderAddress == add)
            {
                localSent = true;
            }
        }
        if (senderAddress == _socket->localAddress())
            localSent = true;
//        qDebug() << _socket->localAddress().toString();
//        qDebug() << "***********************************";
//        qDebug()<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" ;

       if (localSent == false)
            emit DatagramReceived(senderAddress,routesReceived);

    }

}


void ClientDiscovery::sendNewDatagram(QList<Client *> routesList )
{

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);

    // écriture du nombre de routes

    // écriture de la liste des routes

//    qDebug()<< "BS---------------------------------------" ;
//    qDebug()<< "BROADCAST SEND" ;

    qDebug() << "BROADCASTING" << routesList.size() << "hosts";

    out << (quint16) routesList.size();
//    qDebug()<< "routesList Count : " << routesList.count();
    foreach(Client *client, routesList)
    {
        out << client->address().toString();
        out << client->hopNumber();
//        qDebug() << "Address :" << client->address().toString() << " -- hop :" << client->hopNumber();
    }
//    qDebug()<< "---------------------------------------BS" ;
    _socket->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, DISCOVERY_PORT);

}
