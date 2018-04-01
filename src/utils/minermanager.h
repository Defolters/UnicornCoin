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
    void setMinerAddress(QString address);

    //!
    void startMining();

    //!
    void stopMining();

    //!
    virtual void run() override;

signals:
    //!
    void newBlock(QJsonObject block);

private:
    Blockchain *blockchain;
    UnconfirmedPool unconfirmedPool;
    Miner *miner;
    QByteArray prevBlockHash;
    QByteArray minerAddress;
    //QList<QJsonObject> unconfirmed;
};

#endif // MINERMANAGER_H
