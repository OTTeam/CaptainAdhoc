#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QProgressBar>

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
private:
    QLineEdit *address;
    QPushButton *btconnect;
    QPushButton *sendHello;
    QProgressBar *progressBar;

    GestionClients *gestionnaire;
};

#endif // MAINWINDOW_H
