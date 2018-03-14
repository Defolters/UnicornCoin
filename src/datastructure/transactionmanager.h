#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class TransactionManager
{

public:
    //!
    static QByteArray createNewTransaction(QList<double> inputs, QByteArray recipient,
                                    QByteArray privateKey, QByteArray publicKey,
                                    double amount, double fee);
    //!
    static QByteArray signTransaction(QByteArray tx, QByteArray privateKey);

    //!
    static bool verifyTransaction(QJsonObject tx);

};

#endif // TRANSACTIONMANAGER_H
/*
если

in и out тоже json
in:
hash of tx
index in output
script

out:
value
public key


public key + script = my money

выписать прилгательные, наречия и главголы для первого задания.

*/
