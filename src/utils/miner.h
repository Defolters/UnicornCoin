#ifndef MINER_H
#define MINER_H

#include <QObject>
#include <QThread>

/**
 * @brief The Miner class
 * Miner looking for nonce, which will give hash with given difficulty
 */
class Miner : public QThread
{
    Q_OBJECT

public:
    Miner();

    //!
    //! \brief run
    //!
    virtual void run() override;

    //!
    //! \brief forceStop
    //!
    void forceStop();
};

#endif // MINER_H
