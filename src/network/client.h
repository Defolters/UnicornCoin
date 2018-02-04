#ifndef CLIENT_H
#define CLIENT_H

#include <QAbstractSocket>
#include <QHash>
#include <QHostAddress>

#include "server.h"

/**
 * @brief The Client class
 */
class Client : public QObject
{
    Q_OBJECT

public:
    Client();

    //sendMessage();
    /*void sendMessage(const QString &message);
    QString nickName() const;*/
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;  //!< checks that connection is already in multihash

signals:
    /*void newMessage(const QString &from, const QString &message);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);*/

private slots:
    void newConnection(Connection *connection);  //!< Slot is called when server gets new connection
    void connectionError(QAbstractSocket::SocketError socketError);  //!< Slot is called when occured error while writing in socket
    void disconnected();  //!< Slot is called when socket is disconnected
    void readyForUse();  //!< Slot is called when socket is available, so we can add it to multihash

private:
    Server server;  //!< Server for client which listen for new connections
    void removeConnection(Connection *connection);  //!< Method removes connection from peers and deletes connection

    Server server;  //!< Server
    QMultiHash<QHostAddress, Connection *> peers;  //!< MultiHash(dict) which contain all available connections, where key is address of peer and value is Connection
};


#endif // CLIENT_H
