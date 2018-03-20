#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief The TransactionManager class
 */
class TransactionManager
{

public:
    //!
    static QJsonObject createNewTransaction(QList<QJsonObject> inputs, QByteArray recipient,
                                    QByteArray privateKey, QByteArray publicKey, QByteArray address,
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

//https://github.com/nlohmann/json
//http://leveldb.org/
/*
tx:
1. version
2. number of inputs
3. inputs[]
4. number of outputs
5. outputs[]
6. lock_time

input:
1. prev_output:
1) hash of transaction
2) index of output in transaction
2. script_len
3. script

output:
1. value
2. script_len
3. script_pubkey
*/
