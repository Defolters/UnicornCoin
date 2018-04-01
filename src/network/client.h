#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>
#include <QTimer>

#include "server.h"
#include "utils/messagetype.h"

/**
 * @brief The Client Class implements functions of network client.
 * Takes care of @class Connections and hold listening server.
 * Sends and process data.
 */
class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    //!
    //! \brief Sends message to all Connections
    //! \param type
    //! \param message
    //!
    void sendMessage(const MessageType &type, const QByteArray &message);

    //!
    //! \brief Checks that connection is already established
    //! \param host
    //! \return
    //!
    bool hasConnection(const QHostAddress &host) const;

    //!
    //! \brief Method establishes connections with addresses from the string
    //! \param addresses
    //!
    void connectToNodes(const QString &addresses);

signals:
    //!
    //! \brief Signal is emitted, when we get new data
    //! \param type
    //! \param data
    //!
    void newData(const MessageType &type, const QByteArray &data);

    //!
    //! \brief Signal is emitted, when we get request for data
    //! \param type
    //! \param data
    //! \param connection
    //!
    void newRequest(const MessageType &type, const QByteArray &data, Connection *connection);

    //!
    //! \brief Signal is emitted, when we get new connection, for state of network
    //! \param peer
    //!
    void networkPage(int peer);

// change to private
public slots:
    //!
    //! \brief Slot is called when server gets new connection
    //! \param connection
    //!
    void newConnection(Connection *connection);

    //!
    //! \brief Slot is called when connection established, so we can add it to multihash;
    //!
    void readyForUse();

    //!
    //! \brief Slot is called when when we get new data from @class Connection. It emit appropriate signal
    //! \param type
    //! \param data
    //!
    void processData(const MessageType &type, const QByteArray &data);

    //!
    //! \brief Slot is called when error occured while working with socket
    //! \param socketError
    //!
    void connectionError(QAbstractSocket::SocketError socketError);

    //!
    //! \brief Slot is called when socket is disconnected
    //!
    void disconnected();

    //!
    //! \brief Slot is called when it is time to save state of Connections and request Connections for new addresses
    //!
    void getAddr();

private:
    //!
    //! \brief Method removes Connection from peers and deletes it
    //! \param connection
    //!
    void removeConnection(Connection *connection);

    //!
    //! \brief Method returns string with ip of connections
    //! \return
    //!
    QString peersToString();

    //! Dictionary that contains all available connections, where key is address of peer and value is Connection;
    QHash<QString, Connection *> peers;
    //! Timer for getAddr()
    QTimer addrTimer;
    //! Server for client which listen for new connections
    Server server;
};

#endif // CLIENT_H
