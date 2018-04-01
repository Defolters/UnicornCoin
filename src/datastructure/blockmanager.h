#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <QObject>
#include <QJsonObject>

/**
 * @brief The BlockManager class creates and verifies blocks
 */
class BlockManager
{
public:

    //! Method creates block
    static QJsonObject createBlock(QByteArray prevBlockHash,
                                   QByteArray &minerAddress,
                                   QList<QJsonObject> &transactions,
                                   int height);

    //! Method finds merkle root of transactions
    static QByteArray getMerkleRoot(QList<QJsonObject> &transactions);

    static double getCoinbaseReward(int blockchainHeight);

    static double getFeeFromTx(QList<QJsonObject> &transactions);

};

#endif // BLOCKMANAGER_H

/*
block:
1. version
2. prev_block
3. merkle_root
4. timestamp (when_created) //when should I change it in mining?
5. bits (difficulty target)
6. nonce
7. number Of Transactions
8. transactions[] (tx stored here)

tx:
1. version
2. number of inputs
3. inputs[]
4. number of outputs
5. outputs[]
6. lock_time
*/
/*
    QString hash;
    QString prevBlock; //or store hash like QString?
    QString merkleRoot;
    int timestamp;
    int bits;
    int nonce;
    int numberOfTransactions;
    int txns;
*/
/*
Merkle roots do not verify transactions, they verify a set of transactions. Transaction ID's are hashes of the transaction, and the Merkle tree is constructed from these hashes. It means that if a single detail in any of the transactions changes, so does the Merkle root. It also means that if the exact same transactions are listed in a different order, the Merkle root will also change.

So the Merkle root is cryptographic proof of which transactions are in the block, and which order they are in. It provides a convenient piece of information to be included in the block header which is then hashed and included in the next block header. Without the Merkle root in the block header, we would have no cryptographic proof of which transactions are included in a block, nor proof that their contents haven't been tampered with.

Using a Merkle tree is preferable over a hash chain or a hash of concatenated transactions because it allows for a much quicker and simpler test of whether a particular transaction is included in the set. For more details, see the section on Merkle trees in the Developer's Guide.
--
The Merkle tree is not stored anywhere.

It is implicitly defined by the transactions in a block. Whenever needed, the portion of it that is needed is computed on the fly using the transaction identifiers.
*/
