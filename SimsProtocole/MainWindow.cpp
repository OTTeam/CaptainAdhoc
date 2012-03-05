#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

 /*
 * Mise en place de l'UI
 */
    _lbNbClients = new QLabel("Nombre de clients connectés : <strong>0</strong>",this);
    _address = new QLineEdit("127.0.0.1",this);
    _btconnect = new QPushButton("Connect",this);
    _sendHello = new QPushButton("Envoi d'un fichiers",this);

    _progressBar = new QProgressBar(this);
    _progressBar->setMinimum(0);
    _progressBar->setMaximum(100);
    _lbDlSpeed = new QLabel("",this);




    connect(_btconnect,SIGNAL(clicked()),this,SLOT(ConnectClicked()));
    connect(_sendHello, SIGNAL(clicked()),this, SLOT(HelloClicked()));
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(_lbNbClients);
    layout->addWidget(_address);
    layout->addWidget(_btconnect);
    layout->addWidget(_sendHello);


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
    
}




void MainWindow::ConnectClicked()
{
    emit InitiateConnection(QHostAddress(_address->text()));

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
