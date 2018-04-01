#ifndef UNICORNCOIN_H
#define UNICORNCOIN_H

#include <QObject>
#include <QTcpServer>

#include "pages/wallet.h"
#include "utils/keygenerator.h"
#include "utils/messagetype.h"
#include "utils/minermanager.h"
#include "network/client.h"
#include "network/server.h"
#include "network/connection.h"
#include "datastructure/transactionmanager.h"
#include "datastructure/blockchain.h"
#include "datastructure/blockmanager.h"
#include "datastructure/unconfirmedpool.h"

/**
 * \brief The UnicornCoin class
 * The brain of system.
 *
 * \author Defolter
 */
class UnicornCoin : public QObject
{
    Q_OBJECT

public:
    explicit UnicornCoin(QObject *parent = nullptr);

    //!
    //! \brief sendMoney
    //!
    void sendMoney();

    //!
    //! \brief sendMessage
    //! \param data
    //!
    void sendMessage(const QByteArray &data);

    //!
    //! \brief addExistingAddress
    //!
    void addExistingAddress(); //can emit error

    //!
    //! \brief Method generates new address via @class KeyGenerator
    //!
    void generateNewAddress(); //after it make

    //!
    //! \brief Overloaded method with existing private key
    //! \param privateKey
    //!
    void generateNewAddress(QByteArray privateKey); //after it make

    //!
    //! \brief Method checks input variable, �������, ��� ����� ���������� �� �����
    //! and then creates tx with @class TransactionManager
    //! \param recipient
    //! \param amount
    //! \param fee
    //! \param message
    //!
    void createNewTransaction(QString recipient,
                              double amount, double fee,
                              QString message); // ���� + ������� + �������� + ���������

    //!
    //! \brief mineMyMoney
    //!
    void mineMyMoney();

    //!
    //! \brief connectToNode
    //! \param ip
    //!
    void connectToNode(const QString &ip); //conect to ip

    //!
    //! \brief getHistory
    //! \return
    //!
    QList<QJsonObject> getHistory() const;

    //!
    //! \brief getBalance
    //! \return
    //!
    double getBalance() const;

    //!
    //! \brief getPrivateKey
    //! \return
    //!
    QString getPrivateKey();

    //!
    //! \brief getAddress
    //! \return
    //!
    QString getAddress();

signals:
    // Wallet
    //!
    //! \brief newBalance
    //! \param balance
    //! \param unconfirmed
    //!
    void newBalance(double balance, double unconfirmed);

    // Recieve
    //!
    //! \brief newAddress
    //!
    void newAddress();

    // history
    //!
    //! \brief newHistory
    //!
    void newHistory();

    //miner
    // database
    //!
    //! \brief newBlock
    //!
    void newBlock(); // it also contains txs

    // network
    //!
    //! \brief newState
    //!
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
    //!
    //! \brief Slot processes block
    //! \param block
    //!
    void processBlock(QJsonObject block);

private:

    //!
    QTcpSocket *tcpsocket;

    //!
    Client client;  //!< Network client

    //!
    Wallet* wallet;

    //!
    Connection* con;

    //!
    TransactionManager txManager;

    //!
    MinerManager *minerManager;

    //!
    Blockchain *blockchain;

    //!
    UnconfirmedPool *unconfirmed;

    //QMultiHash<address, > unspent; //!< multihash contains unspent money for every address
    QList<QJsonObject> myUnspent; //!< list contains my unspent transactions
    //QList unconfirmed; //!< list of unconfirmed tx sorted by fee

};

#endif // UNICORNCOIN_H

/*
������:
�������� ����������
������� ���������� ��� � �����, � ������� �� ��� � output, � �� ������������ �������� ����� !!!
��������� ����� � ������� from32, ����� ������ ����� ��� ��������� ����
������ ���������� ������� � clipboard
���� add existing key
*/

/*
 * https://royalforkblog.github.io/2014/08/11/graphical-address-generator/#hello
 * privateKey = ''.join(['%x' % random.randrange(16) for x in range(0, 64)]) or hash256(passphrase)
 * secretKey = ecdsa.SigningKey.from_string(privateKey.decode('hex'), curve=ecdsa.SECP256k1)
 * verifyingKey = secretKey.VerifyingKey.toString().encode('hex')
 * publicKey = verifyingKey
 * address = BASE58(RIPEMD160(SHA256(publicKey)))
*/
