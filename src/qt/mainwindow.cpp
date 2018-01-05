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
    //tcpserver = new QTcpServer();
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
    qDebug() << Q_FUNC_INFO;
    qDebug() << ui->lineEdit->text();
    QString ip = ui->ip2->text();
    quint16 port = 0000;
    port = ui->port2->text().toUShort();
    qDebug() << port;
    tcpsocket = new QTcpSocket();
    QHostAddress hostadd("95.71.66.118");
    tcpsocket->connectToHost(hostadd, port);

    qDebug() << tcpsocket->state();
    QByteArray data = "DEBUG";//ui->lineEdit->text();
    //qDebug() << data;
    tcpsocket->write(data);
    qDebug() << tcpsocket->state();

}

void MainWindow::on_listen_clicked()
{
    qDebug() << Q_FUNC_INFO;
    tcpserver = new QTcpServer;

    tcpserver->listen(QHostAddress::Any);
    qDebug() << tcpserver->serverPort();
    qDebug() << tcpserver->serverAddress().toString();
    QObject::connect(tcpserver, SIGNAL(newConnection()),
                     this, SLOT(on_newTcpConnection()));
    /*QString ip = ui->ip->text();
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
    mTcpSocket->connectToHost(hostadd, port);*/
    //tcpserver->connect(mTcpSocket, &QTcpSocket::readyRead, this, &MainWindow::slotServerRead);
}

void MainWindow::slotServerRead()
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "slotRead";
}

void MainWindow::on_newTcpConnection()
{
    qDebug() << Q_FUNC_INFO;
    m_tcpSocket = tcpserver->nextPendingConnection();
    connect(m_tcpSocket, SIGNAL(readyRead()),
            this, SLOT(on_tcpReadyRead()));
//    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(displayError(QAbstractSocket::SocketError)));

//    serverStatusLabel->setText(tr("Accepted connection"));
    //m_tcpServer->close();
    qDebug() << "Sending: Testing new TCP Connection!";
    m_tcpSocket->write("Testing new TCP Connection!");
    //m_state = CONNECTED_TCP_SOCKET;
}

void MainWindow::on_tcpReadyRead()
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << "\t" << qPrintable(m_tcpSocket->readAll());
}
