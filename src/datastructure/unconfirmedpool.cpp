#include "unconfirmedpool.h"

#include <algorithm>
#include <QJsonDocument>
#include <QDebug>

UnconfirmedPool::UnconfirmedPool()
{
    load();
}

void UnconfirmedPool::addTransaction(QJsonObject tx)
{
    unconfirmedPool.append(tx);
}

QList<QJsonObject> UnconfirmedPool::getTransactions(int numberOfTx) const
{
    // sort qlist How to sort QJsonObject?

//    std::sort(unconfirmedPool.begin(), unconfirmedPool.end(), comparator);
    // create new list with txs
    // WHAT IF NUMBER BIGGER THAN SIZE OF LIST????
    QList<QJsonObject> txs = unconfirmedPool.mid(0, numberOfTx);

    return txs;
}

void UnconfirmedPool::removeTransactions(QList<QJsonObject> txs)
{
    //with iterator важно, что мы передаем копию транзакций
    // должен удалять как надо

    QMutableListIterator<QJsonObject> iter(unconfirmedPool);

    while (iter.hasNext() & (!txs.isEmpty()))
    {
        iter.next();

        QMutableListIterator<QJsonObject> iterTxs(txs);

        while(iterTxs.hasNext())
        {
            iterTxs.next();

            if (iter.value()["hash"].toString() ==
                    iterTxs.value()["hash"].toString())
            {
                //delete both
                iter.remove();
                iterTxs.remove();
                break; // break from second while
            }
        }
    }
}

bool UnconfirmedPool::hasTransaction(QJsonObject tx)
{
    // check hashes
    return false;
}

int UnconfirmedPool::size() const
{
    return unconfirmedPool.size();
}

void UnconfirmedPool::save()
{
//    Saves unconfirmedPool into file
//    File unconfirmed.dat
    QFile file("unconfirmedPool.dat");

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream out(&file);
        //
        /*out << unconfirmedPool.size();
        for(int i = 0; i< unconfirmedPool.size(); i++)
        {
            QJsonDocument json(unconfirmedPool.at(i));
            out<<json.toJson();
        }*/
        out << unconfirmedPool;
        file.close();
    }
}

void UnconfirmedPool::load()
{
    QFile file("unconfirmedPool.dat");

    if (file.open(QIODevice::WriteOnly))
    {
        QDataStream in(&file);
        //
        int size = 0;
        in >> size;
        unconfirmedPool.reserve(size);

        for(int i = 0; i< size; i++)
        {
            QByteArray data;
            in >> data;
            QJsonDocument json;
            json.fromJson(data);
            unconfirmedPool.replace(i, json.object());
            //out<<json.toJson();
        }
        //out << unconfirmedPool;
        file.close();
    }
    qDebug() << "FriendList.size() = " << unconfirmedPool.size();
}

bool UnconfirmedPool::comparator(QJsonObject first, QJsonObject second)
{
    if ((first["value"].toDouble() + first["fee"].toDouble()) >
            (second["value"].toDouble() + second["fee"].toDouble()))
    {
        return true;
    }
    else
    {
        return false;
    }
}
