#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QTcpServer>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    tcpserver = new QTcpServer();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    qDebug() << "text";
    ui->statusBar->showMessage("text");
}

void MainWindow::on_pushButton_3_clicked()
{
    qDebug() << ui->lineEdit->text();
    tcpsocket = new QTcpSocket();
    QHostAddress hostadd("192.168.0.102");
    tcpsocket->connectToHost(hostadd, 8333);
    QByteArray data = "DEBUG";//ui->lineEdit->text();
    qDebug() << data;
    tcpsocket->write(data);
}

void MainWindow::on_listen_clicked()
{

    QString ip = ui->ip->text();
    quint16 port = 0000;
    port = ui->port->text().toUShort();
    qDebug() << "ip:" << ip;
    qDebug() << "port:" << port;


    QHostAddress hostadd(ip);
    if (!tcpserver->listen(hostadd, 0)) {
      QMessageBox::critical(this, tr("Fortune Server"), tr("Unable to start the server: %1.").arg(tcpserver->errorString()));
       return;
    }
    mTcpSocket = new QTcpSocket();//mTcpServer->nextPendingConnection();
    mTcpSocket->connectToHost(hostadd, port);
    //tcpserver->connect(mTcpSocket, &QTcpSocket::readyRead, this, &MainWindow::slotServerRead);
}

void MainWindow::slotServerRead()
{
    qDebug() << "slotRead";
}
