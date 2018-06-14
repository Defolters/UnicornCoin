#include "blockchain.h"
#include "../pages/wallet.h"
#include "transactionmanager.h"

#include <QJsonArray>
#include <QJsonObject>
#include <QDebug>

Blockchain::Blockchain() : height (-1) {}

void Blockchain::addBlock(QJsonObject block)
{
    // если это нулевой блок, то кое-какие проверки делать не надо
    QString hash = block["hash"].toString();

    qDebug() << "HaSh: "<<hash;
    // if not 0 block
    if (hash != QString("002eec295dad93f0c31fb3a0a7b4121128091e8f6824e9a1b45cad1150cd5804"))
    {
        checkBlock(block);
    }

    processBlock(block);

    qDebug() << "blockchain";
    qDebug() << blockchain;
}

void Blockchain::save()
{
    // save blockchain state into file
    // save blocks
    //! Saves blockchain (info+hashes), blocks and unspent into file
}

QHash<QString, QPair<QJsonObject, QList<int> > > Blockchain::getMyUnspent(QString address)
{
    // hash of tx, <tx, my outputs>
    QHash<QString, QPair<QJsonObject, QList<int> > > myUnspent;

    QHashIterator<QString, QPair<QJsonObject, QList<int> > > iter(unspent);
    while (iter.hasNext())
    {
        iter.next();

        QJsonObject tx = iter.value().first;
        QList<int> numberOfOutputs;
        QJsonArray outputs = tx["out"].toArray();

        for (int i = 0; i< outputs.size(); i++)
        {
            //if output is unspent
            if (!iter.value().second.contains(i))
                continue;


            QJsonObject out = outputs.at(i).toObject();
            if(out["recipient"].toString() == address)
            {
                numberOfOutputs.append(i);
            }
        }

        if (!numberOfOutputs.isEmpty())
        {
            QPair<QJsonObject, QList<int> > pair;
            pair.first = tx;
            pair.second = numberOfOutputs;
            myUnspent[iter.key()] = pair;
        }
    }
    return myUnspent;
}

int Blockchain::getDifficulty(int currentHeight)
{
    /*if (blockchain.size() == 0)
    {
        throw std::runtime_error("Blockchain is empty");
    }

    // every 2016 blocks
    // analyse blockchain and find new difficulty

    if ((blockchain.size() % 2016) == 0)
    {
        //analyse blockchain time spent on mining and adjust difficulty;
    }
    else
    {
        return blockchain.last()["difficulty"].toInt();
    }*/

    //return QByteArray::fromHex(QString("0100000000000000000000000090000000000000000000000000000000000000").toUtf8()); 1 difficulty
    return 1;
}

int Blockchain::getHeight()
{
    return this->height;
}

QString Blockchain::getBlockHash(int block)
{
    // error prone if we can't load blockchain
    if (block == -1)
    {
        return QString(44,QChar('0'));
    }
    else if ((block > -1) && (block <= height ))
    {
        return blockchain.at(block)["hash"].toString();
    }
    else
    {
        throw std::runtime_error("Wrong block");
    }
}

void Blockchain::checkBlock(QJsonObject block)
{
    // check previous hash
    QString previousHash = block["prev_block"].toString();

    if (previousHash != getBlockHash(height))
    {
        throw std::runtime_error("Previous hash is not valid");
    }

    // check height
    if (block["height"].toInt() != (height+1))
    {
        throw std::runtime_error("Height is not valid");
    }

    // check that all txs unspent
    // move through txs and check if they in unspent (check double spent one txs (two times one hash))
    // we can create txs with similar inputs (miner checks?)
//    QList<QJsonObject> transactions;
    // check that tx inputs == outputs
    // учитывать in, чтобы не было double spendings

    QJsonArray txs = block["txs"].toArray();

    for (auto txjson : txs)
    {
        double insum = 0;
        double outsum = 0;
        QJsonObject tx = txjson.toObject();

        // if block reward, do not verify and check inputs
        if (tx["type"].toInt() == 0) //block reward
        {
            continue;
        }

        // verify tx
        if (!TransactionManager::verifyTransaction(tx))
        {
            throw std::runtime_error("Tx is not valid, so block also");
        }
        else{qDebug() << "Tx is valid";}

        // check inputs
        QJsonArray inputs = tx["in"].toArray();

        for (auto injson : inputs)
        {
            QJsonObject in = injson.toObject();
//            QByteArray hash = QByteArray::fromBase64(in["hash"].toString().toLatin1());
            QString hash = in["hash"].toString();

            int index = in["index"].toInt();

            // есть ли вообще такой хэш
            if (!unspent.contains(hash))
            {
                throw std::runtime_error("Blockchain doesn't contain input");
            }

            QPair<QJsonObject, QList<int>> unspPair = unspent.value(hash);

            // есть ли такой индекс
            if (!unspPair.second.contains(index))
            {
                throw std::runtime_error("unspPair doesn't contain index");
            }

            // считаем количество входных данных
            insum += unspPair.first["out"].toArray().at(index).toObject()["value"].toDouble();

        }

        // добавить проверку: input == output + fee
    }

}

void Blockchain::processBlock(QJsonObject block)
{
    // если высота неподходящая, то выйти (чтобы нулевой блок несколько раз не пихать)
    if (block["height"].toInt() != height+1)
    {
        throw std::runtime_error("Invalid height");
    }

    // достать из блока транзакции и проверить их
    QJsonArray array = block["txs"].toArray();

    for (auto jsonval : array)
    {
        QJsonObject tx = jsonval.toObject();

        // delete inputs from unspent
        if (tx["type"].toInt() != 0)
        {
            QJsonArray inputs = tx["in"].toArray();

            for (auto inputjson : inputs)
            {
                QJsonObject in = inputjson.toObject();

                unspent[in["hash"].toString()].second.removeOne(in["index"].toInt());

                if (unspent.value(in["hash"].toString()).second.isEmpty())
                {
                    unspent.remove(in["hash"].toString());
                }
            }
        }

        // add outputs into unspent
        QPair<QJsonObject, QList<int>> unspentPair;

        unspentPair.first = tx;

        for (int i =0; i < tx["out"].toArray().size(); i++)
        {
            unspentPair.second.append(i);
        }

        unspent[tx["hash"].toString()] = unspentPair;
        qDebug() << "HASH of tx" << tx["hash"].toString();
    }
    /////
    blockchain.append(block);
    height++;
}
