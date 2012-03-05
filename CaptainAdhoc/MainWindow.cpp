#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <iostream>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
#ifdef TRACE
    qDebug() << "[CONS] MainWindow";
#endif
    /*
 * Mise en place de l'UI
 */
    lbNbClients = new QLabel("Nombre de clients connectés : <strong>0</strong>",this);
    address = new QLineEdit("127.0.0.1",this);
    btconnect = new QPushButton("Connexion",this);
    btdisconnect = new QPushButton("Déconnexion",this);
    sendHello = new QPushButton("Envoi d'un fichiers",this);

    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);
    lbDlSpeed = new QLabel("",this);

    _wifi = new WifiConnection();

    connect( _wifi, SIGNAL(ConnectionStatusChanged(int)), this, SLOT(onConnectionStatusChanged(int)) );
    connect( _wifi, SIGNAL(ConnectionFail(int)), this, SLOT(onConnectionFail(int)) );
    connect( btconnect, SIGNAL(clicked()), _wifi, SLOT(Connect()) );
    connect( btdisconnect, SIGNAL(clicked()), _wifi, SLOT(Disconnect()) );
    connect( sendHello, SIGNAL(clicked()), this, SLOT(HelloClicked()) );
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(lbNbClients);
    layout->addWidget(address);
    layout->addWidget(btconnect);
    layout->addWidget(btdisconnect);
    layout->addWidget(sendHello);


    QHBoxLayout *dlLayout = new QHBoxLayout();

    dlLayout->addWidget(progressBar);
    dlLayout->addWidget(lbDlSpeed);

    layout->addLayout(dlLayout);
    this->setLayout(layout);

    /*
     * Création du gestionnaire de clients
     */
    _gestionnaire = new GestionClients(this);

    connect(this,SIGNAL(InitiateConnection(QHostAddress)),_gestionnaire,SLOT(newConnectionRequest(QHostAddress)));
    connect(_gestionnaire,SIGNAL(TransfertUpdate(int)),this,SLOT(UpdateProgress(int)));
    connect(_gestionnaire,SIGNAL(ClientNumberChanged(int)),this,SLOT(UpdateClientsNumber(int)));
    connect(_gestionnaire,SIGNAL(NetworkSpeedUpdate(int)),this,SLOT(UpdateDlSpeed(int)));
}

MainWindow::~MainWindow()
{
#ifdef TRACE
    qDebug() << "[DEST] MainWindow";
#endif
    delete _wifi;
}


void MainWindow::HelloClicked()
{
    _gestionnaire->sendToAll();
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



void MainWindow::onConnectionStatusChanged(int status)
{
    switch (status)
    {
    case FORMED:
        qDebug() << "Notification received : network formed";
        //TODO : lancer le broadcast
        break;
    case CONNECTED:
        qDebug() << "Notification received : connected to network";
        //TODO : lancer le broadcast
        break;
    case DISCONNECTED:
        qDebug() << "Notification received : disconnected from network";
        //TODO : arreter le broadcast
        break;
    }
}

void MainWindow::onConnectionFail(int reason)
{
    switch(reason)
    {
    case DOT11_ADHOC_CONNECT_FAIL_DOMAIN_MISMATCH:
        qDebug() << "Notification received : connection fail (domain mismatch)";
        break;
    case DOT11_ADHOC_CONNECT_FAIL_PASSPHRASE_MISMATCH:
        qDebug() << "Notification received : connection fail (pwd mismatch)";
        break;
    case DOT11_ADHOC_CONNECT_FAIL_OTHER:
        qDebug() << "Notification received : connection fail";
        break;
    default:
        QMessageBox msgBox;
        msgBox.setText("La connexion au réseau a échoué.");
        msgBox.exec();
    }
}

