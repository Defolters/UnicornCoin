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
