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
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> getMyUnspent(QString address);


private:
    QList<QJsonObject> blockchain;
    //! <number of ouput>
    //QHash<QByteArray, QPair<QJsonObject, QList<int>>> myUnspent;
    //! <hash of tx, (tx, list of numbers of outputs)>
    QHash<QByteArray, QPair<QJsonObject, QList<int>>> unspent;

};

#endif // BLOCKCHAIN_H
