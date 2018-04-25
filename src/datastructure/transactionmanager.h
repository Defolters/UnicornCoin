#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

/**
 * \brief The TransactionManager class creates and verifies transactions
 *
 */
class TransactionManager
{

public:
    //! \brief Method creates new transaction
    //! \param inputs
    //! \param recipient Address of recipient
    //! \param privateKey Private key of sender
    //! \param publicKey Public key of sender
    //! \param address ...?
    //! \param amount
    //! \param fee
    //! \return QJsonObject with 6-7 fields
    //!
    static QJsonObject createNewTransaction(QList<QJsonObject> inputs,
                                            QString recipient,
                                            QByteArray privateKey,
                                            QByteArray publicKey,
                                            QString address,
                                            double amount, double fee,
                                            QString message);

    //! \brief Method creates coinbase transaction
    //! This transaction has only one output ... additional info
    //! \param recipient
    //! \param amount
    //! \return
    //!
    static QJsonObject createCoinbaseTransaction(QString recipient,
                                                 double amount);

    //! \brief Method signs the transaction
    //! \param tx Transaction
    //! \param privateKey Private key of sender
    //! \return
    //!
    static QByteArray signTransaction(QByteArray tx, QByteArray privateKey);

    //! \brief Method verifies signature of transaction using public key in tx
    //! \param tx Transaction
    //! \return
    //!
    static bool verifyTransaction(QJsonObject tx);

};

#endif // TRANSACTIONMANAGER_H
/*
 * //! \param type 1 is tx, 0 is block reward(coinbase+fee)
���� ������ ����, �� �� ������� �� unspent ����������
�������� ���� ��������� � ����������

in � out ���� json
in:
hash of tx
index in output
script

out:
value
public key


public key + script = my money

�������� �������������, ������� � �������� ��� ������� �������.

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
