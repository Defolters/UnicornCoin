#include "blockmanager.h"
#include "../datastructure/transactionmanager.h"

#include <QDateTime>
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QCryptographicHash>



QJsonObject BlockManager::createBlock(QString prevBlockHash,
                                      QByteArray &minerAddress,
                                      QList<QJsonObject> transactions,
                                      int height, int difficulty)
{
    /*
    prev_block = "000000000000000117c80378b8da0e33559b5997f2ad55e2f7d18ec1975b9717";
    mrkl_root = "871714dcbae6c8193a2bb9b2a69fe1c0440399f38d94b3a0f1b447275a29978a";
    time_ = 0x53058b35; // 2014-02-20 04:57:25;
    bits = 0x19015f53;
    nonce;
    transaction;
    block;*/

    // first coinbase tx (block reward)
    QJsonObject coinbase = TransactionManager::createCoinbaseTransaction(minerAddress,
                                                                         getCoinbaseReward(height) +
                                                                         getFeeFromTx(transactions));
    // Put coinbase tx at 0 position
    transactions.insert(0, coinbase);

    QJsonObject block;
    block["prev_block"] = prevBlockHash;
    block["merkle_root"] = QString::fromLatin1(getMerkleRoot(transactions).toBase64());

    int time = QDateTime::currentDateTimeUtc().toTime_t();
    block["time"] = time;
    qDebug() << "currentDateTimeUtc().toTime_t()" << QDateTime::currentDateTimeUtc().toTime_t();

    block["height"] = height+1;
    block["difficulty"] = difficulty; // number of zeros (hash should be less, than target) (in miner we create bytearray full of zeros and then put 1 at difficulty position)

    block["nonce"] = -1; // this field changed by miner


    QJsonArray txs;
    for (auto tx : transactions)
    {
        txs << tx;
    }

    block["txs"] = txs;

    return block;
}

bool BlockManager::checkBlock(QJsonObject block)
{
    // проверить поля блока
    /*
    проверить поля
    проверить сигнатуру
    проверить транзакции
    */
    // get hash and drop

    // CHECK HASH
    QString hashFromBlock = block["hash"].toString();
    block.remove(QString("hash"));
    qDebug() << "HASH!!!";
    qDebug() << hashFromBlock;
    QJsonDocument blockDoc(block);

    QByteArray hash = QCryptographicHash::hash(blockDoc.toJson(), QCryptographicHash::Sha3_256);

    if (hashFromBlock != QString(hash.toHex()))
    {
        throw std::runtime_error("Hash is not valid");
    }
    else
    {
        qDebug() << "Hash is valid";
    }

    // CHECK MERKLE ROOT
    QJsonArray array = block["txs"].toArray();
    QList<QJsonObject> txs;
    for (auto tx : array){
        txs.append(tx.toObject());
    }
    QByteArray merkleRoot = getMerkleRoot(txs);


    qDebug() << "MERKLE!!!";
    qDebug() << block["merkle_root"].toString();
    qDebug() << QString::fromLatin1(merkleRoot.toBase64());
    if (block["merkle_root"].toString() != QString::fromLatin1(merkleRoot.toBase64()))
    {
        throw std::runtime_error("Merkle root is not valid");
    }
    else
    {
        qDebug() << "Merkle root is valid";
    }

    // CHECK DIFFICULTY
    int difficulty = block["difficulty"].toInt();
    QString targetStr = QString(64, QChar('0'));
    targetStr.replace(difficulty, 1, "1");

    qDebug() << "Difficulty: " << difficulty << " targetStr: " << targetStr;

    QByteArray target = QByteArray::fromHex(targetStr.toUtf8());

    if (hash >= target)
    {
        throw std::runtime_error("Hash hasn't needed difficulty");
    }
    else
    {
        qDebug() << "Hash have needed difficulty";
    }

    return false;
}

QByteArray BlockManager::getMerkleRoot(QList<QJsonObject> &transactions)
{
    // check if empty
    if (transactions.isEmpty())
    {
        return QByteArray();
    }

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
            QByteArray hash;
            hash = QCryptographicHash::hash(hashes->at(2*i) + hashes->at(2*i+1),
                                            QCryptographicHash::Sha3_256);
            temp->append(hash);
        }

        // если нечетное, то добавляем последнее
        if (hashes->size()%2 != 0) { temp->append(hashes->last()); }

        delete hashes;
        hashes = temp;
    }

    qDebug() << QString::fromLocal8Bit(hashes->first());
    return hashes->first();
}

double BlockManager::getCoinbaseReward(int blockchainHeight)
{
    // block reward half every 400,000 blocks
    int interval = 400000;
    double reward = 0;
    /*while (1)
    {*/
    int halvings = blockchainHeight / interval;

    // ДО ПЯТИ НУЛЕЙ
    if (halvings >= 19){return 0;}

    double subsidy = 50;

//    subsidy >>= halvings;
    for (int i = 0; i < halvings; i++)
        subsidy /= 2;

    //blockchainHeight++;
    //
    qDebug() << (((float)((int)((subsidy) * (100000) ) ) / (100000) ) );
//    }
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
