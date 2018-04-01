#include "unconfirmedpool.h"

#include <QtAlgorithms>

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
    // sort qlist
    qSort(unconfirmedPool.begin(), unconfirmedPool.end(), comparator);
    // create new list with txs
    // WHAT IF NUMBER BIGGER THAN SIZE OF LIST????
    QList<QJsonObject> txs = unconfirmedPool.mid(0, numberOfTx);

    return txs;
}

void UnconfirmedPool::removeTransactions(QList<QJsonObject> txs)
{
    //with iterator важно, что мы передаем копию транзакций
    // должен удалять как надо

    QMutableListIterator<QJsonObject> iter(unspent);

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

int UnconfirmedPool::size() const
{
    return unconfirmedPool.size();
}

void UnconfirmedPool::save()
{

}

void UnconfirmedPool::load()
{

}

bool UnicornCoin::comparator(const QJsonObject &first, const QJsonObject &second)
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
