#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

/**
 * \brief The TransactionManager class
 */
class TransactionManager
{

public:
    //! \brief createNewTransaction
    //! \param type 1 is tx, 0 is block reward(coinbase+fee)
    //! \param inputs
    //! \param recipient
    //! \param privateKey
    //! \param publicKey
    //! \param address
    //! \param amount
    //! \param fee
    //! \return
    //!
    static QJsonObject createNewTransaction(QList<QJsonObject> inputs,
                                            QByteArray recipient,
                                            QByteArray privateKey,
                                            QByteArray publicKey,
                                            QByteArray address,
                                            double amount, double fee,
                                            QString message);

    //! \brief createCoinbaseTransaction
    //! \param recipient
    //! \param amount
    //! \return
    //!
    static QJsonObject &createCoinbaseTransaction(QByteArray recipient,
                                            double amount);

    //! \brief signTransaction
    //! \param tx
    //! \param privateKey
    //! \return
    //!
    static QByteArray signTransaction(QByteArray tx, QByteArray privateKey);

    //! \brief verifyTransaction
    //! \param tx
    //! \return
    //!
    static bool verifyTransaction(QJsonObject tx);

};

#endif // TRANSACTIONMANAGER_H
/*
если пришел блок, то мы удаляем из unspent транзакции
добавить поле сообщения к транзакции

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
