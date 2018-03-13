#include "unicorncoin.h"

UnicornCoin::UnicornCoin(QObject *parent) :
    QObject(parent),
    wallet(new Wallet(this)),
    tcpsocket(nullptr),
    con(nullptr),
    txManager(nullptr)

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

void UnicornCoin::createNewTransaction(QString recipient, int amount, int fee)
{
    /*
        проверить, что адрес и сумма правильные, иначе выкинуть ошибку
        проверить, что у нас на счету (наши unspent деньги) достаточно денег и сформировать массив in, иначе выкинуть ошибку
        создать транзанкцию, добавить ее в unconfirmed и рассказать о ней всем
    */
    //KeyGenerator::checkAddress(recipient);
    // формируем лист из хэшей транзакций и номеров outputs
    // если денегнедостаточно, то выбрасываем ошибку
    try
    {
//        TYPE input = wallet->checkMoney(amount+fee);
        // QPair<hash, numberOfOutput>
        // list of values, which >= amont+fee
//        QList<int> listOfOutputs;
    }
    catch(...)
    {
        //s; // недостаточно денег
    }
    QList<int> listOfOutputs;
    listOfOutputs.push_back(1);
    listOfOutputs.push_back(2);
    listOfOutputs.push_back(3);
    txManager.createNewTransaction(listOfOutputs, recipient, amount, fee);
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
