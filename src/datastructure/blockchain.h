#ifndef BLOCKCHAIN_H
#define BLOCKCHAIN_H

#include <QList>
#include <QJsonObject>
#include <QPair>
#include <QHash>
#include <QList>

/**
 * @brief The Blockchain class
 * This class contains mined blocks and some methods for working with them
 */
class Blockchain
{
public:
    Blockchain();

    //!
    //! \brief Method adds block into blockchain
    //! \param block
    //!
    void addBlock(QJsonObject block);

    //!
    //! \brief Method save blockchain into file
    //!
    void save();

    //!
    //! \brief Method load blockchain from file
    //!
    void load();

    //!
    //! \brief Method returns address' unspent transactions
    //! \param address
    //! \return
    //!
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> getMyUnspent(QString address);

    //!
    //! \brief Method analyses blockchain for time spent on mining and every
    //! 2016 blocks (every week) adjust difficulty
    //! \param currentHeight Parameter if we need find previous difficulty
    //! \return Difficulty that means number of zeros in hash
    //!
    int getDifficulty(int currentHeight = 0);

    //!
    //! \brief Method returns height of blockchain
    //! \return
    //!
    int getHeight();

    //!
    //! \brief Method returns hash of last block in blockchain
    //! \return
    //!
    QString getLastBlockHash();

private:
    //! Blockchain which contains blocks
    QList<QJsonObject> blockchain;

    //! Height of blockchain (= size)
    int height;

    //! <number of ouput>
    //QHash<QByteArray, QPair<QJsonObject, QList<int>>> myUnspent;

    //! <hash of tx, (tx, list of numbers of outputs)> weird thing
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> unspent;
};

#endif // BLOCKCHAIN_H
