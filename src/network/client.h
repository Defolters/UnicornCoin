#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>

#include "utils/messagetype.h"
#include "server.h"
#include "peermanager.h"

/**
 * @brief The Client class.
 * Works with Connections. Sends data.
 */
class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    //! Send message to all Connections, which is established
    void sendMessage(const MessageType type, const QString &message);
    //! checks that connection is already established (in multihash)
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;

signals:
    //! Signal is emitted, when some socket sends new data
    void newData(const MessageType type, const QString &data);
    //! Signal is emitted, when some socket sends requst for data
    void newRequest(const MessageType type, const QString &data, Connection* connection);

private slots:
    //! Slot is called when server gets new connection
    void newConnection(Connection *connection);
    //! Slot is called when error has occured while writing in socket
    void connectionError(QAbstractSocket::SocketError socketError);
    //! Slot is called when socket is disconnected
    void disconnected();
    //! Slot is called when connection established, so we can add it to multihash;
    void readyForUse();

private:
    //! Method process new data and emit neccessary signal
    //void processData();
    //! Method removes connection from peers and deletes connection
    void removeConnection(Connection *connection);
    //! MultiHash(dict) which contain all available connections, where key is address of peer and value is Connection;
    QMultiHash<QHostAddress, Connection *> peers;
    Server server;  //!< Server for client which listen for new connections
    PeerManager peerManager;  //!< manager which makes new connections
};


#endif // CLIENT_H
