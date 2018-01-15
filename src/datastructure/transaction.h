#ifndef TRANSACTION_H
#define TRANSACTION_H
#include <vector>
#include <QString>
#include "data.h"
struct Input
{
    QString hashOfTransaction;
    int indexOfOutputInTransaction;
    int scriptLen;
    QString script;
};

struct Output
{
    double value;
    int scriptLen;
    QString scriptPubKey;
};

class Transaction : public Data
{
public:
    Transaction();
    ~Transaction();
    virtual bool makeRawData() override;
    virtual bool sendToNetwork() override;
    bool addOutput();

private:
    //fields
    double fee;
    QString payTo;
    double value;
    int numberOfInputs;
    std::vector<Input*> inputs;
    int numberOfOutputs;
    std::vector<Output*> outputs;
};

#endif // TRANSACTION_H

/*
tx:
1. version
2. number of inputs
3. inputs[]
4. number of outputs
5. outputs[]
6. lock_time

input:
1. prev_output:
1) hash of transaction
2) index of output in transaction
2. script_len
3. script

output:
1. value
2. script_len
3. script_pubkey
*/
