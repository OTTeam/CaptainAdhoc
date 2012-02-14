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

    connect(btconnect,SIGNAL(clicked()),this,SLOT(ConnectClicked()));
    connect(sendHello, SIGNAL(clicked()),this, SLOT(HelloClicked()));
    QVBoxLayout *layout = new QVBoxLayout(this);

    layout->addWidget(lbNbClients);
    layout->addWidget(address);
    layout->addWidget(btconnect);
    layout->addWidget(sendHello);
    layout->addWidget(progressBar);
    this->setLayout(layout);

/*
 * Création du gestionnaire de clients
*/
    gestionnaire = new GestionClients(this);

    connect(this,SIGNAL(InitiateConnection(QString)),gestionnaire,SLOT(newConnectionRequest(QString)));
    connect(gestionnaire,SIGNAL(TransfertUpdate(int)),this,SLOT(UpdateProgress(int)));
    connect(gestionnaire,SIGNAL(ClientNumberChanged(int)),this,SLOT(UpdateClientsNumber(int)));
 }

MainWindow::~MainWindow()
{
    
}




void MainWindow::ConnectClicked()
{
    emit InitiateConnection(address->text());

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
