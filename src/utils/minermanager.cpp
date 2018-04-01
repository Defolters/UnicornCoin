#include "minermanager.h"
#include "../datastructure/blockmanager.h"

#include <QDebug>

MinerManager::MinerManager(Blockchain *blockchain, UnconfirmedPool *unconfirmedPool) :
    QThread(), blockchain(blockchain), unconfirmedPool(unconfirmedPool)
{
}

void MinerManager::startMining()
{
    qDebug() << Q_FUNC_INFO;

    // в бесконечном цикле (пока нет interrupted) создавать блоки и майнить


    // создать блок
    // отправить его в майнер
    /*miner = new Miner();
    Miner *m_miner = new Miner(b2, 0, 20000000000000);
    m_miner->moveToThread(m_miner);
    m_miner->start();*/
    //qDebug() << unconfirmed.size();
}

void MinerManager::run()
{
    while(1)
    {
        // create list of txs when we have enough txs ??
        QList;
        QJsonObject block;
        // create block
        block = BlockManager::createBlock(blockchain->getLastBlockHash(),
                                          minerAddress,
                                          unconfirmedPool.getTransactions(100),
                                          blockchain->getHeight(),
                                          blockchain->getDifficulty());
        // run miner
        miner = new Miner();
        miner->start();
        // how miner will emit new block
        // emit newBlock


    }
}
