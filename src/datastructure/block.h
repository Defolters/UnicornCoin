#ifndef BLOCK_H
#define BLOCK_H
#include <QString>

class Block
        : Data
{
public:
    Block();
    ~Block();
private:
    QString prevBlock; //or store hash like QString?
    QString merkleRoot;
    int timestamp;
    int bits;
    int nonce;
    int numberOfTransactions;
    int txns;
};

#endif // BLOCK_H

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
