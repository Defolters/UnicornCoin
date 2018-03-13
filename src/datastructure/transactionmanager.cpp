#include "transactionmanager.h"
#include <QDebug>

TransactionManager::TransactionManager(QObject *parent) : QObject(parent)
{

}

QByteArray TransactionManager::createNewTransaction(QList<int> inputs, QString recipient, int amount, int fee)
{
    QJsonObject tx;
    tx["type"] = 1;
    tx["fee"] = fee;

    qDebug() << tx;

    QJsonArray in;

    for (int i =0;i<inputs.size();i++)
    {
        QJsonObject input;
        input["value"] = inputs.at(i);
        input["pubkey"] = "my pubkey";

        in << input;
    }
    // in добавить потом, проверить, что хэш до и после добавления одинаковый
    tx["in"] = in;

    QJsonArray out;
    QJsonObject output;
    output["value"] = amount;
    output["recipient"] = recipient;
    // if sum of inputs > amount + fee
    // add in output


    qDebug() << tx;
    return QByteArray();
}
