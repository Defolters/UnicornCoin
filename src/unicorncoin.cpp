#include "unicorncoin.h"
#include "utils/base32.hpp"
#include <QJsonDocument>

UnicornCoin::UnicornCoin(QObject *parent) :
    QObject(parent),
    wallet(new Wallet(this)),
    tcpsocket(nullptr),
    con(nullptr),
    blockchain(new Blockchain()),
    unconfirmed(new UnconfirmedPool())
{
    wallet->load();
    minerManager = new MinerManager(blockchain, unconfirmed);
    // CREATE TEST FIRST BLOCK

    QByteArray recipient = QByteArray::fromBase64(QString("YZMFC6IDNEDUH25BGRDFYZXICIHXWZDAIJQ65RA").toLatin1());

    // first difficulty is equal to 1
    QList<QJsonObject> list;
    QJsonObject block = BlockManager::createBlock(blockchain->getLastBlockHash(),
                                                  recipient,
                                                  list,
                                                  0,
                                                  1);
    processBlock(block);

    Miner *m_miner = new Miner(block);

    connect(m_miner, SIGNAL(newBlock(QJsonObject)),
            this, SLOT(processBlock(QJsonObject)));

    m_miner->moveToThread(m_miner);
    m_miner->start();
    /*QJsonObject s;
    unconfirmed.append(s);
    mineManager = new MinerManager();

    QJsonObject a;
    unconfirmed.append(a);
    mineManager->moveToThread(mineManager);
    mineManager->run();
    mineManager->setUnconfirmed(unconfirmed);*/
    /*connect(&client, SIGNAL(newData(MessageType,QString)),
            this, SLOT(newData(MessageType,QString)));
    connect(&client, SIGNAL(newRequest(MessageType,QString,Connection*)),
            this, SLOT(newRequest(MessageType,QString,Connection*)));
    connect(&client,     SIGNAL(networkPage(int)),
            this, SLOT(networkPage(int)));*/
    connect(&client, SIGNAL(newData(MessageType,QByteArray)),
            this, SLOT(processData(MessageType,QByteArray)));
    connect(&client, SIGNAL(newRequest(MessageType,QByteArray,Connection*)),
            this, SLOT(processRequest(MessageType,QByteArray,Connection*)));
    connect(minerManager, SIGNAL(newBlock(QJsonObject)),
            this, SLOT(processBlock(QJsonObject)));
    //symbol \ for new line of code
}

void UnicornCoin::sendData(const QByteArray &data)
{
    if (con != nullptr)
    {
        con->sendMessage(MessageType::TX, data);
    }
    else
    {
        qDebug() << "tcpsocket == nullptr";
    }
}

void UnicornCoin::sendData(const MessageType &type, const QByteArray &data)
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
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> unsp = blockchain->getMyUnspent(add);
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

    QJsonObject tx = txManager.createNewTransaction(listOfOutputs, recipientAddr,
                                                    wallet->getPrivateKey(), wallet->getPublicKey(),
                                                    wallet->getAddress(), amount, fee, message);

    qDebug()<<"Size of tx class: "<< sizeof(tx);

    // добавляем в unconfirmed и рассылаем другим людям

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

void UnicornCoin::processBlock(QJsonObject block)
{
    // проверить блок и все транзакции (они валидны и потрачены правильно (с ссылками на правильные оутпуты))
    // если правильно, то добавить в блокчейн, а он сам все транзакции реаспределит как надо
    try
    {
        BlockManager::checkBlock(block); // проверяем на правильность поля
        blockchain->addBlock(block); // проверяем аутпуты и распределяем транзкации по контейнерам

    }
    catch(...)
    {

    }

    // рассказать другим
    QJsonDocument blockDoc(block);
    this->sendData(MessageType::BLOCK, blockDoc.toJson());
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
    this->sendData(MessageType::TX, txDoc.toJson());
}

void UnicornCoin::processData(const MessageType &type, const QByteArray &data)
{
    // process type and data
}

void UnicornCoin::processRequest(const MessageType &type, const QByteArray &data, Connection *connection)
{
 //process type and data and then send back
}
