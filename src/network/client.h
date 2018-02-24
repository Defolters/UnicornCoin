#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QTimer>

#include "server.h"
#include "utils/messagetype.h"

/**
 * @brief Class implements functions of network client.
 * Takes care of @class Connections and hold listening server.
 * Sends and process data.
 */
class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    //! Sends message to all Connections
    void sendMessage(const MessageType &type, const QString &message);
    //! Checks that connection is already established
    bool hasConnection(const QHostAddress &host) const;
    //! Method establishes connections with addresses from the string
    void connectToNodes(const QString &addresses);

signals:
    //! Signal is emitted, when we get new data
    void newData(const MessageType &type, const QString &data);
    //! Signal is emitted, when we get request for data
    void newRequest(const MessageType &type, const QString &data, Connection *connection);
    //! Signal is emitted, when we get new connection, for state of network
    void networkPage(int peer);

// change to private
public slots:
    //! Slot is called when server gets new connection
    void newConnection(Connection *connection);
    //! Slot is called when connection established, so we can add it to multihash;
    void readyForUse();
    //! Slot is called when when we get new data from @class Connection. It emit appropriate signal
    void processData(const MessageType &type, const QString &data);
    //! Slot is called when error occured while working with socket
    void connectionError(QAbstractSocket::SocketError socketError);
    //! Slot is called when socket is disconnected
    void disconnected();
    //! Slot is called when it is time to save state of Connections and request Connections for new addresses
    void getAddr();

private:
    //! Method removes Connection from peers and deletes it
    void removeConnection(Connection *connection);
    //! Method returns string with ip of connections
    QString peersToString();
    //! Dictionary that contains all available connections, where key is address of peer and value is Connection;
    QHash<QString, Connection *> peers;
    //! Timer for getAddr()
    QTimer addrTimer;
    //! Server for client which listen for new connections
    Server server;
};

#endif // CLIENT_H
