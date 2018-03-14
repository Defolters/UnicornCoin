#ifndef UNICORNCOIN_H
#define UNICORNCOIN_H

#include <QObject>
#include <QTcpServer>

#include "pages/wallet.h"
#include "utils/keygenerator.h"
#include "utils/messagetype.h"
#include "network/client.h"
#include "network/server.h"
#include "network/connection.h"
#include "datastructure/transactionmanager.h"

class UnicornCoin : public QObject
{
    Q_OBJECT

public:
    explicit UnicornCoin(QObject *parent = nullptr);

    //!
    void sendMoney();

    //!
    void sendMessage(const QString &data);

    //!
    void addExistingAddress(); //can emit error

    //! Method generates new address via @class KeyGenerator
    void generateNewAddress(); //after it make

    //! Method checks input variable, считает, что денег достаточно на счете
    //! and then creates tx with @class TransactionManager
    void createNewTransaction(QString recipient, double amount, double fee); // кому + сколько + комиссия

    //!
    void mineMyMoney();

    //!
    void connectToNode(const QString &ip); //conect to ip

    QList<QJsonObject> getHistory() const;
    double getBalance() const;
    QByteArray getPrivateKey() const;
    QByteArray getAddress() const;

signals:
    // Wallet
    //!
    void newBalance(double balance, double unconfirmed);
    // Recieve
    //!
    void newAddress();
    // history
    void newHistory();
    //miner
    // database
    void newBlock(); // it also contains txs
    // network
    void newState();

/*
Wallet:
    Balance
    Unconfirmed
Send:
    Create tx
    (if error)
Recieve:
    My address
    add new address
    generate new address
History:
    list of txs with my address
Miner:
    ...
Database:
    list of txs and blocks
Network:
    state of network
    connect to new Node
    (for testing ) send message
*/
public slots:

private:
    QTcpSocket *tcpsocket;
    Client client;  //!< Network client
    Wallet* wallet;
    Connection* con;
    TransactionManager txManager;
    //QMultiHash<address, > unspent; //!< multihash contains unspent money for every address
    QList<QJsonObject> myUnspent; //!< list contains my unspent transactions
    //QList unconfirmed; //!< list of unconfirmed tx sorted by fee

};

#endif // UNICORNCOIN_H
