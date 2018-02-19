#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QTimer>

#include "server.h"
#include "utils/messagetype.h"

/**
 * @brief The Client class.
 * Take care of @class Connections. Sends and process data.
 */
class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    //! Sends message to all Connections, which is alive
    void sendMessage(const MessageType &type, const QString &message);
    //! Checks that connection is already established (in multihash)
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;
    //! Method establishes connections with addresses from the string
    void connectTo(QString &addresses);

signals:
    //! Signal is emitted, when some socket sends new data
    void newData(const MessageType &type, const QString &data);
    //! Signal is emitted, when some socket sends requst for data
    void newRequest(const MessageType &type, const QString &data, Connection *connection);

private slots:
    //! Slot is called when server gets new connection
    void newConnection(Connection *connection);
    //! Slot is called when connection established, so we can add it to multihash;
    void readyForUse();
    //! Slot is called when when we get new data from @class Connection. It emit appropriate signal
    void processData(const MessageType &type, const QString &data);
    //! Slot is called when error occured while writing in socket
    void connectionError(QAbstractSocket::SocketError socketError);
    //! Slot is called when socket is disconnected
    void disconnected();
    //! Slot is called when it is time to save state of @class Connections and request Connections for new
    void getAddr();


private:
    //! Method removes Connection from peers and deletes it
    void removeConnection(Connection *connection);
    //! MultiHash(dict) which contain all available connections, where key is address of peer and value is Connection;
    QMultiHash<QHostAddress, Connection *> peers;
    //! Timer for getAddr()
    QTimer addrTimer;
    //! Server for client which listen for new connections
    Server server;
};

#endif // CLIENT_H
