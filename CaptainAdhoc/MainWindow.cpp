#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
#ifdef TRACE
    qDebug() << "[CONS] MainWindow";
#endif
    /*
 * Mise en place de l'UI
 */
    _lbNbClients = new QLabel("Nombre de clients connectés : <strong>0</strong>",this);
    _btconnect = new QPushButton("Connexion",this);
    _btdisconnect = new QPushButton("Déconnexion",this);
    _sendHello = new QPushButton("Envoi d'un fichiers",this);

    _progressBar = new QProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(100);
    _lbDlSpeed = new QLabel("",this);

    _wifi = new WifiConnection();

    connect( _wifi, SIGNAL(ConnectionStatusChanged(int)), this, SLOT(onConnectionStatusChanged(int)) );
    connect( _wifi, SIGNAL(ConnectionFail(int)), this, SLOT(onConnectionFail(int)) );
    connect( _btconnect, SIGNAL(clicked()), _wifi, SLOT(Connect()) );
    connect( _btdisconnect, SIGNAL(clicked()), _wifi, SLOT(Disconnect()) );
    connect( _sendHello, SIGNAL(clicked()), this, SLOT(HelloClicked()) );
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(_lbNbClients);
    layout->addWidget(_btconnect);
    layout->addWidget(_btdisconnect);
    _btdisconnect->setEnabled(false);
    layout->addWidget(_sendHello);
    _sendHello->setEnabled(false);

    QHBoxLayout *dlLayout = new QHBoxLayout();

    dlLayout->addWidget(_progressBar);
    dlLayout->addWidget(_lbDlSpeed);

    layout->addLayout(dlLayout);
    this->setLayout(layout);

    /*
     * Création du gestionnaire de clients
     */
    _gestionnaire = new GestionClients(this);

    connect(this, SIGNAL(InitiateConnection(QHostAddress)), _gestionnaire, SLOT(newConnectionRequest(QHostAddress)));
    connect(_gestionnaire, SIGNAL(ClientNumberChanged(int)), this, SLOT(UpdateClientsNumber(int)));

    connect(_gestionnaire, SIGNAL(ClientDownloadUpdate(Client *, int)) , this, SLOT(UpdateClientProgress(Client *, int)));
    connect(_gestionnaire, SIGNAL(ClientUploadUpdate(Client *, int)) , this, SLOT(UpdateClientProgress(Client *, int)));

    connect(_gestionnaire, SIGNAL(ClientDownloadSpeedUpdate(Client *, int)) , this, SLOT(UpdateDlSpeed(Client*, int)));
    connect(_gestionnaire, SIGNAL(ClientUploadSpeedUpdate(Client *, int)) , this, SLOT(UpdateDlSpeed(Client*, int)));
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
    _lbNbClients->setText(text);
}

void MainWindow::UpdateClientProgress(Client *client, int Progress)
{
    Q_UNUSED(client)
    _progressBar->setValue(Progress);
}


void MainWindow::UpdateDlSpeed(Client *client, int bytesPerSec)
{
    Q_UNUSED(client)
    QString text;
    if (bytesPerSec > 1000000)
    {
        double dbytesPerSec = bytesPerSec;
        text = QString::number(dbytesPerSec/1000000,'f',1) + " Mo/s";
    }else  if (bytesPerSec > 10000)
    {
        text = QString::number(bytesPerSec/1000) + " Ko/s";
    }
    else
    {
        text = QString::number(bytesPerSec) + " o/s";
    }

    _lbDlSpeed->setText(text);
}



void MainWindow::onConnectionStatusChanged(int status)
{
    switch (status)
    {
    case FORMED:
        qDebug() << "Notification received : network formed";
        _btconnect->setEnabled(false);
        _btdisconnect->setEnabled(true);
        _sendHello->setEnabled(true);
        //TODO : lancer le broadcast
        break;
    case CONNECTED:
        qDebug() << "Notification received : connected to network";
        _btconnect->setEnabled(false);
        _btdisconnect->setEnabled(true);
        _sendHello->setEnabled(true);
        //TODO : lancer le broadcast
        break;
    case DISCONNECTED:
        qDebug() << "Notification received : disconnected from network";
        _btconnect->setEnabled(true);
        _btdisconnect->setEnabled(false);
        _sendHello->setEnabled(false);
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

