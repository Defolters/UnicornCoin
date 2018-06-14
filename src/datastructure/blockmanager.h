#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <QObject>
#include <QJsonObject>

/**
 * @brief The BlockManager class creates and verifies blocks
 * Hash of block saved as hex string
 */
class BlockManager
{
public:

    //!
    //! \brief Method creates block
    //! \param prevBlockHash Hash of previous block
    //! \param minerAddress Address in which mining reward will be sent
    //! \param transactions List with transactions
    //! \param height Height of blockchain
    //! \param difficulty Difficulty of mining
    //! \return
    //!
    static QJsonObject createBlock(QString prevBlockHash,
                                   QString minerAddress,
                                   QList<QJsonObject> transactions,
                                   int height, int difficulty);

    //!
    //! \brief Method validates block
    //! \param block
    //! \return
    //!
    static bool checkBlock(QJsonObject block);

    //!
    //! \brief Method finds merkle root of transactions
    //! \param transactions List of transactions
    //! \return
    //!
    static QString getMerkleRoot(QList<QJsonObject> &transactions);

    //!
    //! \brief Method count coinbase reward for mining new block
    //! coinbase adds new money into system
    //! \param blockchainHeight Height of blockchain
    //! \return
    //!
    static double getCoinbaseReward(int blockchainHeight);

    //!
    //! \brief Method finds the total amount of fee from txs
    //! \param transactions List of transactions
    //! \return Total amount of fee
    //!
    static double getFeeFromTx(QList<QJsonObject> &transactions);
};

#endif // BLOCKMANAGER_H
