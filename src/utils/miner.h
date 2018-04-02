#ifndef MINER_H
#define MINER_H

#include <QObject>
#include <QThread>
#include <QJsonObject>
/**
 * @brief The Miner class
 * Miner looking for nonce, which will give hash with given difficulty
 */
class Miner : public QThread
{
    Q_OBJECT

public:
    Miner(QJsonObject block);

    //!
    //! \brief run
    //!
    virtual void run() override;

    //!
    //! \brief forceStop
    //!
    void forceStop();

signals:
    //!
    //! \brief Signal is emitted when nonce is found
    //! \param block
    //!
    void newBlock(QJsonObject block);

    void error();

private:
    QJsonObject block;
    QByteArray target;
    bool stopFlag;
};

#endif // MINER_H
