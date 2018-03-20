#include "blockmanager.h"
#include <QDateTime>
#include <QDebug>

QJsonObject BlockManager::createBlock(QByteArray prevBlockHash, QList<QJsonObject> transactions)
{
    /*
    prev_block = "000000000000000117c80378b8da0e33559b5997f2ad55e2f7d18ec1975b9717";
    mrkl_root = "871714dcbae6c8193a2bb9b2a69fe1c0440399f38d94b3a0f1b447275a29978a";
    time_ = 0x53058b35; // 2014-02-20 04:57:25;
    bits = 0x19015f53;
    nonce;
    transaction;
    block;*/
    QJsonObject block;
    block["prev_block"];
    block["merkle_root"];
    QDateTime dt;
    block["time"] =dt.toMSecsSinceEpoch();
    qDebug() << dt.toMSecsSinceEpoch();
    block["bits"];
    block["nonce"];
    //transactions.insert();
    QList<int> a;
    a.append(1);
    a.append(2);
    a.append(3);
    qDebug() << a;
    a.insert(0,0);
    qDebug() << a;
    block["txs"];

    return block;
}

QByteArray BlockManager::getMerkleRoot(QList<QJsonObject> transactions)
{
    return QByteArray();
}
