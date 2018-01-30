#include "wallet.h"
#include <QDebug>
#include "qt/mainwindow.h"
#include <fstream>
Wallet::Wallet(MainWindow *parent)
    : parent(parent)
{
    //load();
    exportFile["address"] = {{"private", "priv"},{"public","pub"},{"address", "add"}};
    exportFile["amount"] = {{"amount",100},{"unconfirmed", 10}};
    exportFile["history"] = { {"currency", "USD"}, {"value", 42.99} };
    /*json j;
    i >> j;*/
    //

    std::ifstream i("file.json");
    if(i.is_open())
    {
        //load data
    }
    else
    {
        //set "wallet is unload"
    }
    //o << std::setw(4) << exportFile << std::endl;
    // write prettified JSON to another file
//    std::ofstream o("pretty.json");
    //o <<   std::setw(4) << exportFile << std::endl;
    //std::cout << exportFile.dump(4) << std::endl;
    //this->path = path;
    //    when initialise, i should read file in order to update info and be ready to do smt useful
}

void Wallet::load()
{
    // read a JSON file
    /*std::ifstream i("file.json");
    json j;
    i >> j;

    // write prettified JSON to another file
    std::ofstream o("pretty.json");
    o << std::setw(4) << j << std::endl;*/
    //open file
    //load keys
    //load wallet
    //load history
//    load inforamtion from file
//    update address in recieve page
//    update wallet
//    update history

}

void Wallet::update(MainWindow *mw, QString &priv, QString &pub, QString &addr, double amount, QString &history)
{
    qDebug() << Q_FUNC_INFO;
//    ui->addressRP->setText(addr);
    //        update recieve information
    mw->change_data(addr, "addressRP");

    //        look at block and count amount and history, then write it in the wallet file
}

void Wallet::updateNewKeys(MainWindow *mw, QString &priv, QString &pub, QString &addr)
{
    QString history("");
    update(mw, priv, pub, addr, 0.0, history);
    //        save keys in wallet.dat
    updateFile(priv, pub, addr, 0.0);
}


bool Wallet::updateFile(QString &priv, QString &pub, QString &addr, double amount)
{
    //create file
    //write to file: private, public, address, amount
    return false;
}

bool Wallet::setAmount(double &amount)
{
    this->amount = amount;
    return false;
    //write new amount to file
    // display on the screen
}

bool Wallet::updateWallet(double amount, double unconfirmed)
{
    return false;
//    mw->changeData(amount, "balanceAmountrWP");
//    mw->changeData(unconfirmed, "unconfirmedAmountrWP");
}
