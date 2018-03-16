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

    //
}

void UnicornCoin::createNewTransaction(QString recipient, double amount, double fee)
{
    // спросить у валлет, что у нас есть все ключи и адреса
    //проверить, что адрес и сумма правильные, иначе выкинуть ошибку
    //KeyGenerator::checkAddress(recipient);
    if ((amount <= 0) || (fee<0) ) // || правильное написание адреса (символы нормальные) \\ длина адреса подходящая
    {
        throw std::runtime_error("invalid field");
    }


    // если денегнедостаточно, то выбрасываем ошибку
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
    // HOW TO CAST STRING TO PROPER ADDRESS???
    QByteArray rec = recipient.toUtf8();
    QByteArray tx = txManager.createNewTransaction(listOfOutputs, rec, wallet->getPrivateKey(), wallet->getPublicKey(), wallet->getAddress(), amount, fee);

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
