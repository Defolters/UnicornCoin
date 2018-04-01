#ifndef MINERMANAGER_H
#define MINERMANAGER_H

#include "miner.h"
#include <QJsonObject>
#include "../datastructure/blockchain.h"
#include "../datastructure/unconfirmedpool.h"

/**
 * @brief The MinerManager class
 * MinerManager creates block via blockmanager and start miner to mine block.
 * When nonce is found, class emits new Block.
 */
class MinerManager : public QThread
{
    Q_OBJECT

public:
    MinerManager(Blockchain *blockchain, UnconfirmedPool *unconfirmedPool);

    //!
    //! \brief Method sets address of miner, who will receive reward from mining
    //! \param address
    //!
    void setMinerAddress(QString address);

    //!
    //! \brief Method starts mining
    //!
    void startMining();

    //!
    //! \brief Method stops mining
    //!
    void stopMining();

    //!
    //! \brief Overrided method of QThread
    //!
    virtual void run() override;

signals:
    //!
    //! \brief Signal is emitted when new block is mined
    //! \param block New block
    //!
    void newBlock(QJsonObject block);

private:
    //! Blockchain which contains mined blocks
    Blockchain *blockchain;

    //! Pool with unconfirmed transactions
    UnconfirmedPool unconfirmedPool;

    //! Miner
    Miner *miner;

    //! Address of miner
    QByteArray minerAddress;
};

#endif // MINERMANAGER_H
