#include "wallet.h"
#include <QDebug>
#include "qt/mainwindow.h"
Wallet::Wallet(QString path)
{
    this->path = path;
    //    when initialise, i should read file in order to update info and be ready to do smt useful
}

void Wallet::load(MainWindow *mw)
{
//    load inforamtion from file
//    update address in recieve page
//    update wallet
//    update history

}

void Wallet::update(MainWindow *mw, QString &priv, QString &pub, QString &addr, double &amount, QString &history)
{
    qDebug() << Q_FUNC_INFO;
//    ui->addressRP->setText(addr);
    //        update recieve information
    mw->change_data(addr, "addressRP");

    //        look at block and count amount and history, then write it in the wallet file
}

void Wallet::updateNewKeys(MainWindow *mw, QString &priv, QString &pub, QString &addr)
{
    update(mw, priv, pub, adddr, 0.0, "");
    //        save keys in wallet.dat
    updateFile(priv, pub, addr, 0.0);
}


bool Wallet::updateFile(QString &priv, QString &pub, QString &addr, double &amount)
{
    //create file
    //write to file: private, public, address, amount
}

bool Wallet::setAmount(double &amount)
{
    this->amount = amount;
    //write new amount to file
    // display on the screen
}

bool Wallet::updateWallet(double amount, double unconfirmed)
{
    mw->changeData(amount, "balanceAmountrWP");
    mw->changeData(unconfirmed, "unconfirmedAmountrWP");
}
