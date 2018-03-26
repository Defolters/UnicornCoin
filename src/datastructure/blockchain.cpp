#include "blockchain.h"
#include <QJsonArray>
#include <QJsonObject>
#include "../pages/wallet.h"

Blockchain::Blockchain()
{

}

void Blockchain::addBlock(QJsonObject block)
{
    blockchain.append(block);
    // проверить блок
    // достать из блока транзакции
    QJsonArray array = block["txs"].toArray();
    //QHash<QByteArray, QPair<QJsonObject, QList<int>>> unspent;
    for (auto jsonval : array)
    {
        QJsonObject tx = jsonval.toObject();
        //myUnspent.append(tx.toObject());
        QPair<QJsonObject, QList<int>> unconfirmedPair;

        unconfirmedPair.first = tx;

        for (int i =0; i < tx["out"].toArray().size(); i++)
        {
            unconfirmedPair.second.append(i);
        }

        unspent[QByteArray::fromBase64(tx["hash"].toString().toLatin1())] = unconfirmedPair;
    }

}

    QHash<QByteArray, QPair<QJsonObject, QList<int>>> Blockchain::getMyUnspent(QString address)
{
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> myUnspent;

    QHashIterator<QByteArray, QPair<QJsonObject, QList<int> > > iter(unspent);
    while (iter.hasNext()) {
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
