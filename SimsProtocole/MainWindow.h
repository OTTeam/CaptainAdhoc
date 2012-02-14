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
    void InitiateConnection(QString);
private slots:
    void ConnectClicked();
    void HelloClicked();
    void UpdateClientsNumber(int);
    void UpdateProgress(int);
private:
    QLineEdit *address;
    QPushButton *btconnect;
    QPushButton *sendHello;
    QProgressBar *progressBar;
    QLabel *lbNbClients;

    GestionClients *gestionnaire;
};

#endif // MAINWINDOW_H
