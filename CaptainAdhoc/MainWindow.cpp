#include "MainWindow.h"
#include <QVBoxLayout>
#include <QMessageBox>

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
