#include "WifiConnection.h"
#include "utils.h"

#include <QDebug>


WifiConnection::WifiConnection()
{
    _manager = new WifiManager();
    _network = NULL;
}

WifiConnection::~WifiConnection()
{
    Disconnect();
    delete _manager;
    delete _network;
}

void WifiConnection::Connect()
{
    _manager->RegisterNotifications();
    QList<WifiInterface*> * interfaceList;
    interfaceList = _manager->GetInterfaces();
    qDebug() << "Got" << interfaceList->count() << "interface(s) :";
    foreach (WifiInterface * intface,*interfaceList)
    {
        qDebug() << "Interface :" << intface->GetName();
        qDebug() << "Radio :" << ((intface->IsRadioOn())?"ON" : "OFF");
        switch(intface->GetStatus())
        {
        case CONNECTED:
            qDebug() << "Status : Connecté";
            intface->DisconnectWifi();
            break;
        case FORMED:
            qDebug() << "Status : En attente";
            break;
        case DISCONNECTED:
            qDebug() << "Status : Déconnecté";
            break;
        }
    }

    _manager->DeleteInterfaceList(interfaceList);

    QList<WifiNetwork*> * netList;
    netList = _manager->GetNetworks();
    qDebug() << "Got" << netList->count() << "network(s) :";
    bool found =false;
    foreach (WifiNetwork * net,*netList)
    {
        qDebug() << "Network :" << net->GetSSID();
        if (net->GetSSID() == ADHOC_SSID)
        {
            _network = net;
            _network->RegisterNetworkNotifications();
            _network->Connect(ADHOC_PWD);
            found=true;
        }
    }

    if (found)
    {
        netList->removeOne(_network);
    }

    _manager->DeleteNetworkList(netList);

    if (!found)
    {
        _network = _manager->CreateWifi(ADHOC_SSID,ADHOC_PWD);
        _network->RegisterNetworkNotifications();
    }

    connect(_network, SIGNAL(ConnectionStatusChanged(int)), this, SLOT(onConnectionStatusChanged(int)));
}

void WifiConnection::Disconnect()
{
    _network->Disconnect();
}
