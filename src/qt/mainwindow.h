#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include "Server.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_listen_clicked();
    void slotServerRead();
    void on_newTcpConnection();
    void on_tcpReadyRead();
private slots:
    void on_pushButton_4_clicked();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
    QTcpSocket *mTcpSocket;
    QTcpSocket *m_tcpSocket;
    server server;
};

#endif // MAINWINDOW_H
