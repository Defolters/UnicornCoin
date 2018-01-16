#include "wallet.h"
#include <QDebug>
#include "qt/mainwindow.h"
Wallet::Wallet(QString path)
{
    this->path = path;
//    when initialise, i should read file in order to update info and be ready to do smt useful
}

void Wallet::update(MainWindow *mw, QString &priv, QString &pub, QString &addr)
{
    qDebug() << Q_FUNC_INFO;
//    ui->addressRP->setText(addr);
    //        update recieve information
    mw->change_data(addr, "addressRP");
    //        save keys in wallet.dat
    updateFile();
    //        look at block and count amount and history, then write it in the wallet file
}


bool Wallet::updateFile()
{
    //create file
    //write to file: private, public, address, amount
}
