#include "unicorncoin.h"
#include "utils/base32.hpp"

UnicornCoin::UnicornCoin(QObject *parent) :
    QObject(parent),
    wallet(new Wallet(this)),
    tcpsocket(nullptr),
    con(nullptr)
{
    wallet->load();
    // CREATE TEST BLOCK
    QList<QJsonObject> txs;
    QJsonObject one;
    one["value"] = 100;
    one["recipient"] = "YZMFC6IDNEDUH25BGRDFYZXICIHXWZDAIJQ65RA";
    one["hash"] = "me";

    txs.append(one);

    QJsonObject block = BlockManager::createBlock(QByteArray("2"), txs);
    processBlock(block);

    /*connect(&client, SIGNAL(newData(MessageType,QString)),
            this, SLOT(newData(MessageType,QString)));
    connect(&client, SIGNAL(newRequest(MessageType,QString,Connection*)),
            this, SLOT(newRequest(MessageType,QString,Connection*)));
    connect(&client, SIGNAL(networkPage(int)),
            this, SLOT(networkPage(int)));*/
}

void UnicornCoin::sendMessage(const QByteArray &data)
{
    if (con != nullptr)
    {
        con->sendMessage(MessageType::TX, data);
    }
    else {qDebug() << "tcpsocket == nullptr";}
}

void UnicornCoin::generateNewAddress()
{
    QByteArray privk = KeyGenerator::generatePrivateKey();
    generateNewAddress(privk);
}

void UnicornCoin::generateNewAddress(QByteArray privateKey)
{
    QByteArray pubk = KeyGenerator::generatePublicKey(privateKey);
    QByteArray ad = KeyGenerator::generateAddress(pubk);

    if (!KeyGenerator::checkAddress(ad,pubk))
    {
        throw std::runtime_error("Error occurred, try again");
    }

    // update keys in wallet
    wallet->setKeys(privateKey, pubk, ad);

    QString add = base32::toBase32(wallet->getAddress());
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> unsp = blockchain.getMyUnspent(add);
    wallet->setUnspent(unsp);

    // analyze blockchain and search for amount/ouputs !!!!

    /*QList<QJsonObject> unspent = blockManager.getUnspent(ad);
    if (!unspent.empty())
        wallet->setUnspent(unspent);
*/
}

void UnicornCoin::createNewTransaction(QString recipient, double amount, double fee, QString message)
{
    // Check that we have keys
    if (!wallet->isKeySet())
    {
        throw std::runtime_error("You haven't address, you should start with generating one");
    }

    // Check fields
    if (amount <= 0)
    {
        throw std::runtime_error("Amount cannot be less or equal zero");
    }
    if (fee < 0)
    {
        throw std::runtime_error("Fee cannot be less zero");
    }
    if (recipient.size() != 39)
    {
        throw std::runtime_error("Lenght of address is wrong, try another");
    }
    if(!base32::isBase32(recipient))
    {
        throw std::runtime_error("Unsupported characters in address");
    }


    // если денег недостаточно, то выбрасываем ошибку
    //проверить, что у нас на счету (наши unspent деньги) достаточно денег и сформировать массив in, иначе выкинуть ошибку
    QList<QJsonObject> listOfOutputs;
    try
    {
//        TYPE input = wallet->checkMoney(amount+fee);
        // QPair<hash, numberOfOutput>
        // list of values, which >= amont+fee
        listOfOutputs = wallet->checkMoney(amount+fee);
    }
    catch(std::runtime_error ex)
    {
        //s; // недостаточно денег
        throw ex;
    }

    //создать транзанкцию, добавить ее в unconfirmed и рассказать о ней всем

    QByteArray recipientAddr;
    try
    {
        recipientAddr = base32::fromBase32(recipient);
    }
    catch(std::runtime_error ex)
    {
        throw std::runtime_error("Address is wrong, try another");
    }

    QJsonObject tx = txManager.createNewTransaction(1, listOfOutputs, recipientAddr,
                                                    wallet->getPrivateKey(), wallet->getPublicKey(),
                                                    wallet->getAddress(), amount, fee, message);

    qDebug()<<"Size of tx class: "<< sizeof(tx);

    // добавляем в unconfirmed и рассылаем другим людям

    //client.sendMessage();
}

void UnicornCoin::connectToNode(const QString &ip)
{
    quint16 port = 9229;
    qDebug() << port << " " <<ip;
    QHostAddress hostadd(ip);
    con = new Connection();
    con->connectToHost(hostadd, port);
    qDebug() << con->state();
    client.newConnection(con);
}

QList<QJsonObject> UnicornCoin::getHistory() const
{
    return wallet->getHistory();
}

double UnicornCoin::getBalance() const
{
    return wallet->getBalance();
}

QString UnicornCoin::getPrivateKey()
{
    return base32::toBase32(wallet->getPrivateKey());
}

QString UnicornCoin::getAddress()
{
    return base32::toBase32(wallet->getAddress());
}

void UnicornCoin::processBlock(QJsonObject block)
{
    try
    {
        blockchain.addBlock(block);

    }
    catch(...)
    {

    }

}

bool UnicornCoin::comparator(const QJsonObject &first, const QJsonObject &second)
{
    if ((first["value"].toDouble() + first["fee"].toDouble()) >
            (second["value"].toDouble() + second["fee"].toDouble()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
