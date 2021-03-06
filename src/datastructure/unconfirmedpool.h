#ifndef UNCONFIRMEDPOOL_H
#define UNCONFIRMEDPOOL_H

#include <QList>
#include <QJsonObject>

/**
 * \brief The UnconfirmedPool class
 *
 * This class contains list with unconfirmed transactions and methods for
 * working with them.
 *
 */
class UnconfirmedPool
{
public:
    UnconfirmedPool();

    //!
    //! \brief Method adds transaction into unconfirmed pool
    //! \param tx
    //!
    void addTransaction(QJsonObject tx);

    //!
    //! \brief getTransactions Method sorts unconfirmed pool by fee
    //! and then return first numberOfTx transactions
    //! \param numberOfTx
    //! \return
    //!
    QList<QJsonObject> getTransactions(int numberOfTx = 100) const ;

    //!
    //! \brief Method removes transactions from unconfirmedPool
    //! Method is called, when new block is come
    //! \param txs
    //!
    void removeTransactions(QList<QJsonObject> txs);

    //!
    //! \brief Method checks whether is tx in unconfirmed pool
    //! \param tx Transaction
    //! \return
    //!
    bool hasTransaction(QJsonObject tx);

    //!
    //! \brief Method return size of unconfirmed pool
    //! \return
    //!
    int size() const;

    //!
    //! \brief Method saves data into file
    //! Saves unconfirmedPool into file
    //!
    void save();

    //!
    //! \brief load
    //!
    void load();

private:
    //!
    //! \brief Method compares transactions by fee
    //! \param first
    //! \param second
    //! \return
    //!
    static bool comparator(QJsonObject first, QJsonObject second);

    //!
    QList<QJsonObject> unconfirmedPool;
};

#endif // UNCONFIRMEDPOOL_H
