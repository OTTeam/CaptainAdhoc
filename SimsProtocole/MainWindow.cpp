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

    connect(this, SIGNAL(InitiateConnection(QHostAddress)), gestionnaire, SLOT(newConnectionRequest(QHostAddress)));
    connect(gestionnaire, SIGNAL(ClientNumberChanged(int)), this, SLOT(UpdateClientsNumber(int)));

    connect(gestionnaire, SIGNAL(ClientDownloadUpdate(Client *, int)) , this, SLOT(UpdateClientProgress(Client *, int)));
    connect(gestionnaire, SIGNAL(ClientUploadUpdate(Client *, int)) , this, SLOT(UpdateClientProgress(Client *, int)));

    connect(gestionnaire, SIGNAL(ClientDownloadSpeedUpdate(Client *, int)) , this, SLOT(UpdateDlSpeed(Client*, int)));
    connect(gestionnaire, SIGNAL(ClientUploadSpeedUpdate(Client *, int)) , this, SLOT(UpdateDlSpeed(Client*, int)));


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

void MainWindow::UpdateClientProgress(Client *client, int Progress)
{
    Q_UNUSED(client)
    progressBar->setValue(Progress);
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

    lbDlSpeed->setText(text);
}
