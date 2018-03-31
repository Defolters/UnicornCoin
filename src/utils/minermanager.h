#ifndef MINERMANAGER_H
#define MINERMANAGER_H

#include "miner.h"
#include <QJsonObject>

/**
 * @brief The MinerManager class
 * MinerManager creates block via blockmanager and start miner to mine block
 */
class MinerManager : public QThread
{
    Q_OBJECT

public:
    MinerManager();

    //!
    void setMinerAddress(QString address);

    //!
    void startMining();

    //!
    void stopMining();

    //!
    void setUnconfirmed(QList<QJsonObject> &unconfirmed);
    virtual void run() override;

signals:
    //!
    void newBlock(QJsonObject block);

private:
    Miner *miner;
    QByteArray prevBlockHash;
    QByteArray minerAddress;
    QList<QJsonObject> unconfirmed;
};

#endif // MINERMANAGER_H
