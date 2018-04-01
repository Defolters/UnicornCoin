#include "blockmanager.h"
#include "../datastructure/transactionmanager.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>



QJsonObject BlockManager::createBlock(QByteArray prevBlockHash,
                                      QByteArray &minerAddress,
                                      QList<QJsonObject> &transactions,
                                      int height)
{
    /*
    prev_block = "000000000000000117c80378b8da0e33559b5997f2ad55e2f7d18ec1975b9717";
    mrkl_root = "871714dcbae6c8193a2bb9b2a69fe1c0440399f38d94b3a0f1b447275a29978a";
    time_ = 0x53058b35; // 2014-02-20 04:57:25;
    bits = 0x19015f53;
    nonce;
    transaction;
    block;*/

    qDebug() << "ss";
    // first coinbase tx (block reward)
    QJsonObject coinbase = TransactionManager::createCoinbaseTransaction(minerAddress,
                                                                         getCoinbaseReward(height) +
                                                                         getFeeFromTx(transactions));
    // Put coinbase tx at 0 position
    transactions.insert(0, coinbase);

    QJsonObject block;
    block["prev_block"] = QString::fromLatin1(prevBlockHash.toBase64());;
    block["merkle_root"] = QString::fromLatin1(getMerkleRoot(transactions).toBase64());

    QDateTime dt;
    block["time"] = dt.toMSecsSinceEpoch();
    qDebug() << dt.toMSecsSinceEpoch();

    block["height"] = height+1;
    block["bits"] = 1;
    block["nonce"] = 1;


    QJsonArray txs;
    for (auto tx : transactions)
    {
        txs << tx;
    }

    block["txs"] = txs;

    return block;
}

QByteArray BlockManager::getMerkleRoot(QList<QJsonObject> &transactions)
{
    // check if empty
    // if odd +1
    QList<QByteArray> *hashes = new QList<QByteArray>();

    for (int i = 0; 2*i < transactions.size(); i++)
    {
        QJsonDocument data(transactions.at(i));

        hashes->append(QCryptographicHash::hash(data.toJson(),
                                                QCryptographicHash::Sha3_256));
    }
    while(hashes->size() != 1)
    {
        QList<QByteArray> *temp = new QList<QByteArray>();

        for (int i = 0; (2*i) < hashes->size(); i++)
        {
            QByteArray hash = QCryptographicHash::hash(hashes->at(2*i)+ hashes->at(2*i+1),
                                                       QCryptographicHash::Sha3_256);
            temp->append(hash);
        }
        // если нечетное, то добавляем последнее
        if (hashes->size()%2 != 0){temp->append(hashes->last());}
        delete hashes;
        hashes = temp;
    }
    qDebug() << QString::fromLocal8Bit(hashes->first());
    return QByteArray();
}

double BlockManager::getCoinbaseReward(int blockchainHeight)
{
    // block reward half every 300,000 blocks
    int interval = 4;//00000;
    double reward = 0;
    while(1){
    int halvings = blockchainHeight / interval;

    // ДО ПЯТИ НУЛЕЙ
    if (halvings >= 19){break;}
        //return 0;
    double subsidy = 50;

//    subsidy >>= halvings;
    for (int i = 0; i < halvings; i++)
        subsidy /= 2;

    blockchainHeight++;
    qDebug() << (((float)((int)((subsidy) * (100000))) / (100000)));
    }
    return reward;
}

double BlockManager::getFeeFromTx(QList<QJsonObject> &transactions)
{
    double fee;

    for (int i = 0; i < transactions.size(); i++)
    {
        fee += transactions.at(i)["fee"].toDouble();
    }

    return fee;
}
