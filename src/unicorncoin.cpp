#include "unicorncoin.h"

UnicornCoin::UnicornCoin(QObject *parent) :
    QObject(parent),
    wallet(new Wallet(this)),
    tcpsocket(nullptr),
    con(nullptr)
{
    wallet->load();

    /*connect(&client, SIGNAL(newData(MessageType,QString)),
            this, SLOT(newData(MessageType,QString)));
    connect(&client, SIGNAL(newRequest(MessageType,QString,Connection*)),
            this, SLOT(newRequest(MessageType,QString,Connection*)));
    connect(&client, SIGNAL(networkPage(int)),
            this, SLOT(networkPage(int)));*/
}

void UnicornCoin::sendMessage(const QString &data)
{
    if (con != nullptr)
    {
//        con->write(data);
        con->sendMessage(MessageType::TX, data);
    }
    else qDebug() << "tcpsocket == nullptr";
}

void UnicornCoin::generateNewAddress()
{
    QByteArray privk = KeyGenerator::generatePrivateKey();
    QByteArray pubk = KeyGenerator::generatePublicKey(privk);
    QByteArray ad = KeyGenerator::generateAddress(pubk);

    if (!KeyGenerator::checkAddress(ad,pubk))
    {
        throw std::runtime_error("Error occurred, try again");
    }

    // update keys in wallet
    wallet->setKeys(privk, pubk, ad);
    // analyze blockchain and search for amount/ouputs
    /*QList<QJsonObject> unspent = blockManager.getUnspent(ad);
    if (!unspent.empty())
        wallet->setUnspent(unspent);
*/

}

void UnicornCoin::createNewTransaction(QString recipient, double amount, double fee)
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


    // если денег недостаточно, то выбрасываем ошибку
    //проверить, что у нас на счету (наши unspent деньги) достаточно денег и сформировать массив in, иначе выкинуть ошибку
    QList<double> listOfOutputs;
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

    listOfOutputs.push_back(1);
    listOfOutputs.push_back(2);
    listOfOutputs.push_back(3);

    //создать транзанкцию, добавить ее в unconfirmed и рассказать о ней всем

    QByteArray recipientAddr;
    try
    {
        recipientAddr = KeyGenerator::fromBase32(recipient);
    }
    catch(std::runtime_error ex)
    {
        throw std::runtime_error("Address is wrong, try another");
    }

    QByteArray tx = txManager.createNewTransaction(listOfOutputs, recipientAddr, wallet->getPrivateKey(), wallet->getPublicKey(), wallet->getAddress(), amount, fee);

    // добавляем в unspent и рассылаем другим людям
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
    return KeyGenerator::toBase32(wallet->getPrivateKey());
}

QString UnicornCoin::getAddress()
{
    return KeyGenerator::toBase32(wallet->getAddress());
}
