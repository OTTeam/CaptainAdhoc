#include "WifiConnection.h"
#include "utils.h"

#include <QDebug>


WifiConnection::WifiConnection()
{
#ifdef TRACE
    qDebug() << "[CONS] WifiConnection";
#endif
    _manager = new WifiManager();
    _network = NULL;
    _connected = false;
}

WifiConnection::~WifiConnection()
{
#ifdef TRACE
    qDebug() << "[DEST] WifiConnection";
#endif
    Disconnect();
    delete _manager;
    delete _network;
}


void WifiConnection::Connect()
{
    if( !_connected )
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

        connect(_network, SIGNAL(ConnectionStatusChanged(int)), this, SIGNAL(ConnectionStatusChanged(int)));
        connect(_network, SIGNAL(ConnectionStatusChanged(int)), this, SLOT(onConnectionStatusChanged(int)));
        connect(_network, SIGNAL(ConnectionFail(int)), this, SIGNAL(ConnectionFail(int)));
    }
}


void WifiConnection::Disconnect()
{
    if(_connected)
    {
        _network->Disconnect();
    }
}


void WifiConnection::onConnectionStatusChanged(int status)
{
    switch (status)
    {
    case FORMED:
        _connected = true;
        break;
    case CONNECTED:
        _connected = true;
        break;
    case DISCONNECTED:
        _connected = false;
        break;
    }
}
