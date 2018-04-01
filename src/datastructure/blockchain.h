#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H
#include <QList>
#include <QJsonObject>
#include <QPair>
#include <QHash>
#include <QList>

/**
 * @brief The Blockchain class contains blocks and has some important methods like (find tx)
 */
class Blockchain
{
public:
    Blockchain();

    //!
    void addBlock(QJsonObject block);

    //!
    void saveBlockchain();

    //!
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> getMyUnspent(QString address);

    //! Method analyse blockchain for time spent on mining and every 2016
    //! blocks (every week) adjust difficulty (current height for old blocks)
    int getDifficulty(int currentHeight = 0);

    QString getLastBlockHash();

private:
    //!
    QList<QJsonObject> blockchain;

    int height;

    //! <number of ouput>
    //QHash<QByteArray, QPair<QJsonObject, QList<int>>> myUnspent;

    //! <hash of tx, (tx, list of numbers of outputs)>
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> unspent;


};

#endif // BLOCKCHAIN_H
