#include "minermanager.h"
#include "../datastructure/blockmanager.h"

#include <QDebug>

MinerManager::MinerManager(Blockchain *blockchain, UnconfirmedPool *unconfirmedPool) :
    QThread(), blockchain(blockchain), unconfirmedPool(unconfirmedPool)
{
}

void MinerManager::setMinerAddress(QString address)
{
    minerAddress = QByteArray::fromBase64(address.toLatin1());
    qDebug() << "MINERADDRESS" << address.size();
    qDebug() << minerAddress;
    qDebug() << QString::fromLatin1(minerAddress.toBase64()).remove(QString::fromLatin1(minerAddress.toBase64()).size()-1,1);
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

void MinerManager::save()
{

}

void MinerManager::load()
{

}

void MinerManager::run()
{
    Miner *m_miner;
    while(1)
    {
        // create list of txs when we have enough txs ??
        QJsonObject block;
        // create block
        block = BlockManager::createBlock(blockchain->getBlockHash(blockchain->getHeight()),
                                          minerAddress,
                                          unconfirmedPool->getTransactions(100),
                                          blockchain->getHeight(),
                                          blockchain->getDifficulty());
        // run miner
        // block to solve, begin, end
         m_miner = new Miner(block);

        connect(m_miner, SIGNAL(newBlock(QJsonObject)), this, SIGNAL(newBlock(QJsonObject)));

        //m_miner->moveToThread(m_miner);
        //m_miner->start();
        QJsonObject blk = m_miner->mineBlock(block);
        emit newBlock(blk);
        //m_miner->wait(60000);
        //m_miner->exec();
        // how miner will emit new block
        // emit newBlock
        sleep(60);
    }
}

