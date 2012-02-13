#include "MainWindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{

/*
 * Mise en place de l'UI
 */
    address = new QLineEdit("127.0.0.1",this);
    btconnect = new QPushButton("Connect",this);
    sendHello = new QPushButton("Hello!",this);
    progressBar = new QProgressBar(this);
    progressBar->setMinimum(0);
    progressBar->setMaximum(100);

    connect(btconnect,SIGNAL(clicked()),this,SLOT(ConnectClicked()));
    connect(sendHello, SIGNAL(clicked()),this, SLOT(HelloClicked()));
    QVBoxLayout *layout = new QVBoxLayout(this);

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
    connect(gestionnaire,SIGNAL(TransfertUpdate(int)),progressBar,SLOT(setValue(int)));
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
