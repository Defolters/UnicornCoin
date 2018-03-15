#include "wallet.h"
#include <QDebug>
#include "qt/mainwindow.h"
#include <fstream>
#include <QList>
Wallet::Wallet(QObject *parent)
//    : parent(parent)
{
    QJsonObject one;
    one["value"] = 100;
    one["recipient"] = "me";

    myUnspent.append(one);
    //load();
    /*
    exportFile["address"] = {{"private", "priv"},{"public","pub"},{"address", "add"}};
    exportFile["amount"] = {{"amount",100},{"unconfirmed", 10}};
    exportFile["history"] = { {"currency", "USD"}, {"value", 42.99} };
    /*json j;
    i >> j;*/
    //

    /*std::ifstream i("file.json");
    if(i.is_open())
    {
        //load data
    }
    else
    {
        //set "wallet is unload"
    }*/
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


void Wallet::setBalance(double balance)
{
    this->balance = balance;
    updateFile();
}

void Wallet::updateFile()
{
    // save all information into file
}

QList<double> Wallet::checkMoney(double amount) // ���������� ������? �������� �� ����������?
{
    QList<double> listOfOutputs;
    double amountFromUnspent = 0;

    int i = 0;
    while ((amountFromUnspent < amount) && (i<myUnspent.size()))
    {
            listOfOutputs.append(myUnspent.at(i)["value"].toDouble());
            amountFromUnspent += myUnspent.at(i)["value"].toDouble();
            i++;
    }

    if (amountFromUnspent < amount)
    {
        throw std::runtime_error("Not enough money in wallet");
    }


    return listOfOutputs;
}

void Wallet::setKeys(QByteArray privateKey, QByteArray publicKey, QByteArray address)
{
    this->privateKey = privateKey;
    this->publicKey = publicKey;
    this->address = address;
    updateFile();
}

QList<QJsonObject> Wallet::getHistory() const
{
    return history;
}

double Wallet::getBalance() const
{
    return balance;
}

QByteArray Wallet::getPrivateKey() const
{
    return privateKey;
}

QByteArray Wallet::getPublicKey() const
{
    return publicKey;
}

QByteArray Wallet::getAddress() const
{
    return address;
}
