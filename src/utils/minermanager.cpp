#include "minermanager.h"
#include <QDebug>

MinerManager::MinerManager() : QThread()
{
}

void MinerManager::startMining()
{
    qDebug() << Q_FUNC_INFO;

    // � ����������� ����� (���� ��� interrupted) ��������� ����� � �������


    // ������� ����
    // ��������� ��� � ������
    /*miner = new Miner();
    Miner *m_miner = new Miner(b2, 0, 20000000000000);
    m_miner->moveToThread(m_miner);
    m_miner->start();*/
    qDebug() << unconfirmed.size();
}

void MinerManager::setUnconfirmed(QList<QJsonObject> &unconfirmed)
{
    this->unconfirmed = unconfirmed;
}

void MinerManager::run()
{
    while(1)
    {

    }
}
