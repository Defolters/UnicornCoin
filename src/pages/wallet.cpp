#include "wallet.h"
#include <QDebug>
#include "qt/mainwindow.h"
#include <fstream>
#include <QList>
Wallet::Wallet(QObject *parent) :
    keySet(false)
//    : parent(parent)
{
    /*QJsonObject one;
    one["value"] = 100;
    one["recipient"] = "me";

    myUnspent.append(one);*/
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

void Wallet::save()
{
    //save into file keySet, path, privatek, publick, address, balance
    // unconfirmed
    // history
    // unspent
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
    //updateFile();
}


QList<QJsonObject> Wallet::checkMoney(double amount) // возвращать ссылку? Удалится ли содержимое?
{
    if (amount > balance){
        throw std::runtime_error("Not enough money in wallet");
    }

    QList<QJsonObject> listOfOutputs;
    double amountFromUnspent = 0;

    // hash of tx, <tx, outputs>
    QHashIterator<QString, QPair<QJsonObject, QList<int> > > iter(myUnspent);
    while (iter.hasNext())
    {
        iter.next();
        QJsonObject tx = iter.value().first;
        QList<int> numbOfOut = iter.value().second;
        //create listofoutputs
        for (int i = 0; i < numbOfOut.size(); i++)
        {
            QJsonObject out;
            out["hash"] = tx["hash"];
            out["index"] = numbOfOut.at(i);
            out["value"] = tx["out"].toArray().at(numbOfOut.at(i)).toObject()["value"].toDouble();
//            out["script"] = ""

            amountFromUnspent += tx["out"].toArray().at(numbOfOut.at(i)).toObject()["value"].toDouble();
            listOfOutputs.append(out);

            if (amountFromUnspent >= amount)
                break;
        }

        if (amountFromUnspent >= amount)
            break;


    }

    if (amountFromUnspent < amount)
    {
        throw std::runtime_error("Not enough money in wallet");
    }

    this->balance -= amount;
    this->unconfirmed += amount;

    return listOfOutputs;
}

void Wallet::setKeys(QByteArray privateKey, QByteArray publicKey, QByteArray address)
{
    this->privateKey = privateKey;
    this->publicKey = publicKey;
    this->address = address;
    //updateFile();
    keySet = true;
    balance = 0;
}

void Wallet::setUnspent(QHash<QString, QPair<QJsonObject, QList<int>>> unspent)
{
    this->myUnspent = unspent;
    //count money
    balance = 0;
    QHashIterator<QString, QPair<QJsonObject, QList<int> > > iter(unspent);
    while (iter.hasNext())
    {
        iter.next();

        QJsonObject tx = iter.value().first;

        for (int index : iter.value().second)
        {
            balance += tx["out"].toArray()[index].toObject()["value"].toDouble();
        }
    }

    this->balance = balance;
    this->unconfirmed=0;
}

QList<QJsonObject> Wallet::getHistory() const
{
    return history;
}

double Wallet::getBalance() const
{
    return balance;
}

double Wallet::getUnconfirmed() const
{
    return unconfirmed;
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

bool Wallet::isKeySet() const
{
    return keySet;
}
