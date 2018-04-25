#include "unicorncoin.h"
#include "utils/base32.hpp"
#include <QJsonDocument>

UnicornCoin::UnicornCoin(QObject *parent) :
    QObject(parent),
    wallet(new Wallet(this)),
    con(nullptr),
    blockchain(new Blockchain()),
    unconfirmed(new UnconfirmedPool())
{
    wallet->load();
    minerManager = new MinerManager(blockchain, unconfirmed);
    minerManager->setMinerAddress(QString("59ADCRSEDPT6JMRSUI3F6FWNVM63P9P7ZKMGICI"));
    // CREATE FIRST BLOCK BY HAND AND PUT IT INTO BLOCKCHAIN
    /*QJsonObject block;

    QByteArray recipient = QByteArray::fromBase64(QString("59ADCRSEDPT6JMRSUI3F6FWNVM63P9P7ZKMGICI").toLatin1());
    QList<QJsonObject> list;

    block = BlockManager::createBlock(blockchain->getBlockHash(-1),
                                                  recipient,
                                                  list,
                                                  -1,
                                                  1);

    block["time"] = 1500000000;

    Miner *m_miner = new Miner(block);

    connect(m_miner, SIGNAL(newBlock(QJsonObject)),
            this, SLOT(processBlock(QJsonObject)));

    m_miner->moveToThread(m_miner);
    m_miner->start();
*/
    QJsonObject block;
    QJsonDocument blockdoc;

    QFile file("block0.dat");
    if(file.open(QIODevice::ReadOnly)){
        qDebug() << "\nReading from file..";
        //qDebug() << file.readAll();
//        QByteArray json = file.readAll();
//        qDebug() << json;
        QString in = file.readAll();
        QByteArray json = in.toUtf8();
        blockdoc = QJsonDocument::fromJson(json);
//        blockdoc.fromJson(json);
//        qDebug() << blockdoc.toJson();
        block = blockdoc.object();
        qDebug() << block;
        processBlock(block);
    }


    /*connect(&client, SIGNAL(newData(DataType,QString)),
            this, SLOT(newData(DataType,QString)));
    connect(&client, SIGNAL(newRequest(DataType,QString,Connection*)),
            this, SLOT(newRequest(DataType,QString,Connection*)));
    connect(&client,     SIGNAL(networkPage(int)),
            this, SLOT(networkPage(int)));*/
    connect(&client, SIGNAL(newData(DataType,QByteArray)),
            this, SLOT(processData(DataType,QByteArray)));
    connect(&client, SIGNAL(newRequest(DataType,QByteArray,Connection*)),
            this, SLOT(processRequest(DataType,QByteArray,Connection*)));
    connect(minerManager, SIGNAL(newBlock(QJsonObject)),
            this, SLOT(processBlock(QJsonObject)));

    minerManager->moveToThread(minerManager);
    minerManager->start();
    //symbol \ for new line of code
}

void UnicornCoin::sendData(const QByteArray &data)
{
    if (con != nullptr)
    {
        con->sendMessage(DataType::TX, data);
    }
    else
    {
        qDebug() << "tcpsocket == nullptr";
    }
}

void UnicornCoin::sendData(const DataType &type, const QByteArray &data)
{
    client.sendData(type, data);
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
    QHash<QString, QPair<QJsonObject, QList<int>>> unsp = blockchain->getMyUnspent(add);
    wallet->setUnspent(unsp);

    //clear history

    // set miner
    minerManager->setMinerAddress(add);

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

    /*QByteArray recipientAddr;
    try
    {
        recipientAddr = base32::fromBase32(recipient);
    }
    catch(std::runtime_error ex)
    {
        throw std::runtime_error("Address is wrong, try another");
    }*/
    QString add = base32::toBase32(wallet->getAddress());
    QJsonObject tx = TransactionManager::createNewTransaction(listOfOutputs, recipient,
                                                    wallet->getPrivateKey(),
                                                    wallet->getPublicKey(),
                                                    add,
                                                    amount, fee, message);

    qDebug()<<"Size of tx class: "<< sizeof(tx);

    // добавл€ем в unconfirmed и рассылаем другим люд€м

    //client.sendMessage();
    processTransaction(tx);
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

void UnicornCoin::save()
{
    // save data from unicorn + other classes
    client.save();
    wallet->save();
    minerManager->save();
    blockchain->save();
    //unconfirmed->save();
}

void UnicornCoin::load()
{
    // load data to unicorn + other classes
}

void UnicornCoin::processBlock(QJsonObject block)
{
    qDebug() << Q_FUNC_INFO;
    // проверить блок и все транзакции (они валидны и потрачены правильно (с ссылками на правильные оутпуты))
    // если правильно, то добавить в блокчейн, а он сам все транзакции реаспределит как надо
    try
    {
        // удал€€ хэш, удал€ем ли мы его насовсем?
        BlockManager::checkBlock(block); // провер€ем на правильность пол€ (деньги непотрачены, все правильно)
        blockchain->addBlock(block); // провер€ем аутпуты и распредел€ем транзкации по контейнерам
        if (wallet->isKeySet())
        {
            QString add = base32::toBase32(wallet->getAddress());
            wallet->setUnspent(blockchain->getMyUnspent(add));
            emit newBalance(wallet->getBalance(),0);
        }
        QList<QJsonObject> confirmed;
        QJsonArray blocks = block["txs"].toArray();
        for (auto txjson : blocks)
        {
            confirmed.append(txjson.toObject());
        }
        unconfirmed->removeTransactions(confirmed);

        // рассказать другим
        QJsonDocument blockDoc(block);
        this->sendData(DataType::BLOCK, blockDoc.toJson());
    }
    catch(std::runtime_error ex)
    {
        qDebug() << ex.what();
    }
}

void UnicornCoin::processTransaction(QJsonObject tx)
{
    // проверить, что у нас еще нет ее в неподтвержденных (она уникальна)
    if (unconfirmed->hasTransaction(tx))
        return;

    //проверить транзакцию на валидность
    try {TransactionManager::verifyTransaction(tx);}
    catch (std::runtime_error ex){}

    // добавить ее в .. unconfirmed??
    unconfirmed->addTransaction(tx);

    // рассказать другим
    QJsonDocument txDoc(tx);
    this->sendData(DataType::TX, txDoc.toJson());
}

void UnicornCoin::processData(const DataType &type, const QByteArray &data)
{
    // process type and data
}

void UnicornCoin::processRequest(const DataType &type, const QByteArray &data, Connection *connection)
{
 //process type and data and then send back
}
