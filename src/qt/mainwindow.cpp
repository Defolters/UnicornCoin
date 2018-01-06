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
    ui(new Ui::MainWindow), server(new Server)
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    //tcpserver = new QTcpServer();
    server->addUI(this);
    ui->label_3->setText("123");
    ui->statusBar->showMessage("Out of syns");
    QPixmap pixmap(QPixmap(":/res/icons/error.png").scaledToHeight(ui->statusBar->height()/2));

    ui->statusBarIconNetwork->setPixmap(pixmap);
    ui->statusBarIconNetwork->setToolTip(tr("net1"));
    ui->statusBar->addPermanentWidget(ui->statusBarIconNetwork);
//    ui->statusBar->addPermanentWidget(ui->horizontalSlider_2);
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
    //qDebug() << ui->lineEdit->text();
    //qDebug() << tcpsocket->state();

    QByteArray data = ui->lineEdit->text().toUtf8();//ui->lineEdit->text();
    tcpsocket->write(data);
}

void MainWindow::on_listen_clicked()
{
    qDebug() << Q_FUNC_INFO;
    /*tcpserver = new QTcpServer;

    tcpserver->listen(QHostAddress::Any);
    qDebug() << tcpserver->serverPort();
    qDebug() << tcpserver->serverAddress().toString();
    QObject::connect(tcpserver, SIGNAL(newConnection()),
                     this, SLOT(on_newTcpConnection()));*/
    server->start_listen(8333);
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

void MainWindow::change_data(QString str, QString label)
{
    qDebug() << Q_FUNC_INFO;

    qDebug() << str;
    if (label == "label_3")
    {
        ui->label_3->setText(str);
    }
    else if(label == "ip_con")
    {
        ui->ip_con->setText(str);
    }
    else if(label == "port_con")
    {
        ui->port_con->setText(str);
    }

}

void MainWindow::on_pushButton_4_clicked()
{
    QString ip = ui->ip2->text();
    quint16 port = 8333;
    //port = ui->port2->text().toUShort();
    qDebug() << port << " " <<ip;
    tcpsocket = new QTcpSocket();
    QHostAddress hostadd(ip);//"127.0.0.1");("95.71.66.118")
    tcpsocket->connectToHost(hostadd, port);

    qDebug() << tcpsocket->state();
}

void MainWindow::on_actionwallet_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionsend_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionrecieve_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionhistory_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_actiondatabase_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionaddressbook_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_actionminer_triggered()
{
    ui->stackedWidget->setCurrentIndex(6);
}
