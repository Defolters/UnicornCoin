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

class UnicornCoin : public QObject
{
    Q_OBJECT
public:
    explicit UnicornCoin(QObject *parent = nullptr);

    //!
    void sendMoney();
    void sendMessage(const QString &data);
    void addExistingAddress(); //can emit error
    void generateNewAddress(); //after it make qdialog
    void mineMyMoney();
    void connectToNode(const QString &ip); //conect to ip
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
};

#endif // UNICORNCOIN_H
