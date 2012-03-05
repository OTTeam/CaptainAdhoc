#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>

#include "GestionClients.h"

class MainWindow : public QWidget
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void InitiateConnection(QHostAddress);
private slots:
    void ConnectClicked();
    void HelloClicked();
    void UpdateClientsNumber(int);

    void UpdateClientProgress(Client *, int);

    void UpdateDlSpeed(Client *, int);
private:
    QLineEdit *_address;
    QPushButton *_btconnect;
    QPushButton *_sendHello;
    QProgressBar *_progressBar;
    QLabel *_lbNbClients;
    QLabel *_lbDlSpeed;

    GestionClients *_gestionnaire;
};

#endif // MAINWINDOW_H
