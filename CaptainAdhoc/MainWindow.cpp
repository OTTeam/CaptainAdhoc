#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

    /*
 * Mise en place de l'UI
 */
    lbNbClients = new QLabel("Nombre de clients connectés : <strong>0</strong>",this);
    address = new QLineEdit("127.0.0.1",this);
    btconnect = new QPushButton("Connect",this);
    sendHello = new QPushButton("Envoi d'un fichiers",this);

    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    lbDlSpeed = new QLabel("",this);

    ConnectWifi();


    connect(btconnect,SIGNAL(clicked()),this,SLOT(ConnectClicked()));
    connect(sendHello, SIGNAL(clicked()),this, SLOT(HelloClicked()));
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(lbNbClients);
    layout->addWidget(address);
    layout->addWidget(btconnect);
    layout->addWidget(sendHello);


    QHBoxLayout *dlLayout = new QHBoxLayout();

    dlLayout->addWidget(progressBar);
    dlLayout->addWidget(lbDlSpeed);

    layout->addLayout(dlLayout);
    this->setLayout(layout);

    /*
 * Création du gestionnaire de clients
*/
    gestionnaire = new GestionClients(this);

    connect(this,SIGNAL(InitiateConnection(QHostAddress)),gestionnaire,SLOT(newConnectionRequest(QHostAddress)));
    connect(gestionnaire,SIGNAL(TransfertUpdate(int)),this,SLOT(UpdateProgress(int)));
    connect(gestionnaire,SIGNAL(ClientNumberChanged(int)),this,SLOT(UpdateClientsNumber(int)));
    connect(gestionnaire,SIGNAL(NetworkSpeedUpdate(int)),this,SLOT(UpdateDlSpeed(int)));
}

MainWindow::~MainWindow()
{
    cout << "[DEST] MainWindow" << endl;
}

void MainWindow::ConnectWifi()
{
    manager.RegisterNotifications();
    QList<WifiInterface*> * interfaceList;
    interfaceList = manager.GetInterfaces();
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

    manager.DeleteInterfaceList(interfaceList);

    QList<WifiNetwork*> * netList;
    netList = manager.GetNetworks();
    qDebug() << "Got" << netList->count() << "network(s) :";
    bool found =false;
    foreach (WifiNetwork * net,*netList)
    {
        qDebug() << "Network :" << net->GetSSID();
        if (net->GetSSID() == ADHOC_SSID)
        {
            net->Connect(ADHOC_PWD);
            found=true;
        }
    }

    manager.DeleteNetworkList(netList);

    if (!found)
    {
        WifiNetwork * net = manager.CreateWifi(ADHOC_SSID,ADHOC_PWD);
        delete net;
    }

}




void MainWindow::ConnectClicked()
{
    emit InitiateConnection(QHostAddress(address->text()));

}

void MainWindow::HelloClicked()
{
    gestionnaire->sendToAll();
}


void MainWindow::UpdateClientsNumber(int nbClients)
{
    QString text = "Nombre de clients connectés : <strong>";
    text += QString::number(nbClients) + "</strong>";
    lbNbClients->setText(text);
}

void MainWindow::UpdateProgress(int Progress)
{

    progressBar->setValue(Progress);
}


void MainWindow::UpdateDlSpeed(int bytesPerSec)
{
    QString text;
    if (bytesPerSec > 10000)
    {
    }else  if (bytesPerSec > 10000)
    {
        text = QString::number(bytesPerSec/1000) + "Kb/s";
    }
    else
    {
        text = QString::number(bytesPerSec) + "b/s";
    }

    lbDlSpeed->setText(text);
}
