#include "blockchain.h"
#include "../pages/wallet.h"

#include <QJsonArray>
#include <QJsonObject>

Blockchain::Blockchain() : height (0)
{

   QJsonObject debugBlock;
   debugBlock["hash"] = "00000000000000000021cee1e8097955acd09dddf9743e66cb0bb4a4de07fff2";

   blockchain.append(debugBlock);
}

void Blockchain::addBlock(QJsonObject block)
{
    // проверить блок
    blockchain.append(block);

    // достать из блока транзакции и проверить их
    QJsonArray array = block["txs"].toArray();
    //QHash<QByteArray, QPair<QJsonObject, QList<int>>> unspent;
    for (auto jsonval : array)
    {
        QJsonObject tx = jsonval.toObject();
        //myUnspent.append(tx.toObject());
        QPair<QJsonObject, QList<int>> unspentPair;

        unspentPair.first = tx;

        for (int i =0; i < tx["out"].toArray().size(); i++)
        {
            unspentPair.second.append(i);
        }

        unspent[QByteArray::fromBase64(tx["hash"].toString().toLatin1())] = unspentPair;
    }

}

void Blockchain::save()
{
    // save blockchain state into file
    // save blocks
}

QHash<QByteArray, QPair<QJsonObject, QList<int> > > Blockchain::getMyUnspent(QString address)
{
    QHash<QByteArray, QPair<QJsonObject, QList<int> > > myUnspent;

    QHashIterator<QByteArray, QPair<QJsonObject, QList<int> > > iter(unspent);
    while (iter.hasNext())
    {
        iter.next();

        QJsonObject tx = iter.value().first;
        QList<int> numberOfOutputs;
        QJsonArray outputs = tx["out"].toArray();

        for (int i = 0; i< outputs.size(); i++)
        {
            //if in list
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
        //cout << i.key() << ": " << i.value() << endl;

    }

    return myUnspent;
}

int Blockchain::getDifficulty(int currentHeight)
{
    if (blockchain.size() == 0)
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
    }

    //return QByteArray::fromHex(QString("0100000000000000000000000090000000000000000000000000000000000000").toUtf8()); 1 difficulty
    return 1;
}

int Blockchain::getHeight()
{
    return this->height;
}

QString Blockchain::getLastBlockHash()
{
    // error prone if we can't load blockchain
    if (blockchain.size() == 0)
    {
        throw std::runtime_error("Blockchain is empty");
    }

    return blockchain.last()["hash"].toString();
}
