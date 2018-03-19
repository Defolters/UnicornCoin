#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(width(), height());

    ui->statusBar->showMessage("Out of syns");

    ui->statusBarIconNetwork->setToolTip(tr("net1"));
    ui->statusBar->addPermanentWidget(ui->statusBarIconNetwork);
    ui->addressRP->setText("Address is not generated yet");



    /*
    load wallet
    load information from wallet
    update wallet, recieve and history from the wallet
    connect to peers and check new blocks
    start miner

    */
    setFont(QFont ("Calibri Light", 9));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::networkPage(int peer)
{
    ui->lcdNumber->display(peer);
}

void MainWindow::on_pushButton_send_clicked()
{
    qDebug() << Q_FUNC_INFO;
    //qDebug() << ui->lineEdit->text();
    //qDebug() << tcpsocket->state();
    //exportFile["address"] = {{"private", "priv"},{"public","pub"},{"address", "add"}};
    //QByteArray data = ui->lineEdit_2->text().toUtf8();//ui->lineEdit->text();
    QString data = ui->lineEdit_2->text();
    uniCoin.sendMessage(data);
    //if (tcpsocket != nullptr) tcpsocket->write(data);

}

void MainWindow::newData(const MessageType &type, const QString &data)
{

}

void MainWindow::newRequest(const MessageType &type, const QString &data, Connection *connection)
{

}

void MainWindow::on_pushButton_4_clicked()
{
    qDebug() << Q_FUNC_INFO;
    // ÏÐÎÂÅÐÈÒÜ ÍÀ ÏÐÀÂÈËÜÍÎÑÒÜ ÂÂÎÄÀ
    QString ip = ui->ip2_2->text();
    uniCoin.connectToNode(ip);
}

void MainWindow::on_actionwallet_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_actionsend_triggered()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_actionhistory_triggered()
{
    ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::on_actionminer_triggered()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_actiondatabase_triggered()
{
    ui->stackedWidget->setCurrentIndex(4);
}

void MainWindow::on_actionnetwork_triggered()
{
    ui->stackedWidget->setCurrentIndex(5);
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->balanceAmountWP->setText("21000000 UCN");
}

void MainWindow::on_generateNewAddressRP_clicked()
{

    QMessageBox::StandardButton reply = QMessageBox::question(
                this, "IMPORTANT", "If you already generated address, you can "
                                   "lose it!\n Continue?",
                QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        try
        {
            uniCoin.generateNewAddress();
        }
        catch(std::runtime_error ex)
        {
            QMessageBox::critical(this, "ERROR", QString("Error: %1").arg(ex.what()));
            return;
        }


        QString priv = uniCoin.getPrivateKey();
        QString addr = uniCoin.getAddress();
        double balance = uniCoin.getBalance();
        // unconfirmed?
        QList<QJsonObject> history = uniCoin.getHistory();
        qDebug() << "ADDR: " <<addr.size() << addr;

        // copy to clipboard
        //QMessageBox msgBox;
        QMessageBox::warning(this, "IMPORTANT", QString("You should save it! Without this information you cannot use your money.\n"
                                                "Your private key: %1\n"
                                                "Your address: %2").arg(priv, addr));
        // update information
        ui->addressRP->setText(addr);
        ui->balanceAmountWP->setText(QString::number(balance));
        // unconfirmed
        // history
    }
}

void MainWindow::on_createTransaction_clicked()
{
    //Transaction tx(ui->addressSP->text(), ui->amountSP->value(), ui->feeSP->value());

    try
    {
        uniCoin.createNewTransaction(ui->addressSP->text(), ui->amountSP->value(), ui->feeSP->value());
    }
    catch (std::runtime_error ex)
    {
        QMessageBox::critical(this, "ERROR", QString("Error: %1").arg(ex.what()));
    }
    catch(...)
    {
        QMessageBox::critical(this, "ERROR", QString("Undefined error"));
        // save state into file and exit
        exit(1);
    }
    /*QTreeWidgetItem *item = new QTreeWidgetItem();
    item->setText(0, QString::number(wow++));
    item->setText(1,"sds");
    ui->historyTree->addTopLevelItem(item);*/
    ui->tableWidget->insertRow(0);
    ui->tableWidget->setItem( 0, 0, new QTableWidgetItem(QString::number(wow++)));
    ui->tableWidget->setItem( 0, 1, new QTableWidgetItem(ui->addressSP->text()));
    ui->tableWidget->setItem( 0, 2, new QTableWidgetItem(QString::number(ui->amountSP->value())));
    ui->tableWidget->setItem( 0, 3, new QTableWidgetItem(QString::number(ui->feeSP->value())));
}

void MainWindow::on_addExistingAddressRP_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(tr("If you already generated address, you can lose him!\n Continue?"));
    //msgBox.
    QAbstractButton* pButtonYes = msgBox.addButton(tr("Yes"), QMessageBox::YesRole);
    msgBox.addButton(tr("No"), QMessageBox::NoRole);

    msgBox.exec();

    if (msgBox.clickedButton() == pButtonYes) {
        QMessageBox::StandardButton reply = QMessageBox::question(this, "ERROR", "If you already generated address, you can lose him!\n Continue?",
                                                                  QMessageBox::Yes | QMessageBox::No);
    }

//    show window, where user add address and private key, then save, update
}
