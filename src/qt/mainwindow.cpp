#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>
#include "datastructure/transaction.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    tcpsocket(nullptr),
    wallet(new Wallet(this))
{
    ui->setupUi(this);
    setFixedSize(width(), height());
    //tcpserver = new QTcpServer();
    //server->addUI(this);
    ui->label_3->setText("123");
    ui->statusBar->showMessage("Out of syns");
    setFont(QFont ("Calibri Light", 9));
    QPixmap pixmap(QPixmap(":/res/icons/error.png").scaledToHeight(ui->statusBar->height()/2));

    ui->statusBarIconNetwork->setPixmap(pixmap);
    ui->statusBarIconNetwork->setToolTip(tr("net1"));
    ui->statusBar->addPermanentWidget(ui->statusBarIconNetwork);
    wallet->load();

    connect(&client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));

    /*myNickName = client.nickName();
    newParticipant(myNickName);*/

//    ui->statusBar->addPermanentWidget(ui->horizontalSlider_2);
    /*
    load wallet
    load information from wallet
    update wallet, recieve and history from the wallet
    connect to peers and check new blocks
    start miner

    */
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

void MainWindow::on_pushButton_send_clicked()
{
    qDebug() << Q_FUNC_INFO;
    //qDebug() << ui->lineEdit->text();
    //qDebug() << tcpsocket->state();
    //exportFile["address"] = {{"private", "priv"},{"public","pub"},{"address", "add"}};
    try
    {
        json::parse("dsfsd");
    }
    catch(json::exception& ex)
    {
        qDebug() << ex.what();
    }
    QByteArray data = ui->lineEdit_2->text().toUtf8();//ui->lineEdit->text();
    if (tcpsocket != nullptr) tcpsocket->write(data);
    else qDebug() << "tcpsocket == nullptr";
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
    quint16 port = 8333;
    port = ui->port_2->text().toUShort();
    QString ip = ui->ip_2->text();
    qDebug() << port << " " << ip;
    //server->start_listen(ip, port);
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
    /*qDebug() << Q_FUNC_INFO;
    m_tcpSocket = tcpserver->nextPendingConnection();
    connect(m_tcpSocket, SIGNAL(readyRead()),
            this, SLOT(on_tcpReadyRead()));
//    connect(m_tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
//            this, SLOT(displayError(QAbstractSocket::SocketError)));

//    serverStatusLabel->setText(tr("Accepted connection"));
    //m_tcpServer->close();
    qDebug() << "Sending: Testing new TCP Connection!";
    m_tcpSocket->write("Testing new TCP Connection!");
    //m_state = CONNECTED_TCP_SOCKET;*/
}

void MainWindow::on_tcpReadyRead()
{
    /*qDebug() << Q_FUNC_INFO;
    qDebug() << "\t" << qPrintable(m_tcpSocket->readAll());*/
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
        ui->ipCon->setText(str);
    }
    else if(label == "port_con")
    {
        ui->portCon->setText(str);
    }
    else if(label == "addressRP")
    {
        ui->addressRP->setText(str);
    }
    else if(label == "balanceAmountWP")
    {
        ui->balanceAmountWP->setText(str + " UCN");
    }
    else if(label == "unconfirmedAmountrWP")
    {
        ui->unconfirmedAmountrWP->setText(str + " UCN");
    }
}

void MainWindow::dataBack()
{
    QByteArray array = tcpsocket->read(tcpsocket->bytesAvailable());
    qDebug() <<"databack: " <<array;
    if (array == "PING 1 p")
    {
        tcpsocket->write("PONG 1 p");
        //tcpsocket->write("PING 1 p");
    }
}

void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << Q_FUNC_INFO;
    QString ip = ui->ip2_2->text();
    quint16 port = 8333;
    //port = ui->port2->text().toUShort();
    qDebug() << port << " " <<ip;
    tcpsocket = new QTcpSocket();
    QHostAddress hostadd(ip);//"127.0.0.1");("95.71.66.118")
    tcpsocket->connectToHost(hostadd, port);
    QObject::connect(tcpsocket, SIGNAL(readyRead()),this,SLOT(dataBack()));
    qDebug() << tcpsocket->state();
    //client.connectTo();
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

void MainWindow::on_actionminer_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actiondatabase_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_actionnetwork_triggered()
{
    ui->stackedWidget->setCurrentIndex(6);
}
void MainWindow::on_pushButton_2_clicked()
{
    ui->balanceAmountWP->setText("21000000 UCN");
}

void MainWindow::on_send_money_clicked()
{

}

void MainWindow::on_generateNewAddressRP_clicked()
{

    QMessageBox::StandardButton reply = QMessageBox::question(this, "IMPORTANT", "If you already generated address, you can lose him!\n Continue?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {

//        generate pair of keys
        KeyGenerator kg;
        QString priv = kg.generatePrivateKey();
        QString pub = kg.generatePublicKey("1");
        QString addr = kg.generateAddress("1");
//        display keys
        QMessageBox::warning(this, "IMPORTANT", QString("You should save it! Without this information you cannot use your money.\n"
                                                "Your private key: %1\n"
                                                "Your address: %2").arg(priv, addr));
        // update information
        wallet->updateNewKeys(this, priv, pub, addr);
    }
}

void MainWindow::on_createTransaction_clicked()
{
    Transaction tx(ui->addressSP->text(), ui->amountSP->value(), ui->feeSP->value());
    /*bool valid = tx.verify(); // and add inputs
    if valid == true
    tx.sign();
    tx.sendToNetwork();*/
}

void MainWindow::on_addExistingAddressRP_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this, "IMPORTANT", "If you already generated address, you can lose him!\n Continue?",
                                                              QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {}
//    show window, where user add address and private key, then save, update
}
