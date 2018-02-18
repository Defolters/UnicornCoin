#include <QtNetwork>

#include "client.h"
#include "connection.h"

static const int AddrInterval = 5 * 1000;  //!< getAddr interval 5s

Client::Client()
{
    QObject::connect(&server, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));
    /*QObject::connect(&peerManager, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));*/

    // read addresses and establish initial connections
    qDebug() << QDir::currentPath();
    QFile file("addresses.dat");
    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "addresses opened";
        QTextStream addresses(&file);
        qDebug() << file.size() << addresses.readAll();
        //прочитать текст и отправить в функцию
        connectTo(addresses.readAll());
    }

    addrTimer.setInterval(AddrInterval);
    // set timer for a method, which sends request for new addresses and saves addresses
    // через время запрашивать новые адреса и пытаться подключиться, затем текущие соединения сохранить в файл
    QObject::connect(&addrTimer, SIGNAL(timeout()), this, SLOT(getAddr()));
    addrTimer.start();
}

void Client::sendMessage(const MessageType type, const QString &message)
{
    qDebug() << Q_FUNC_INFO;

    if (message.isEmpty())
        return;

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendMessage(message);
}

/*
QString Client::about() const
{
    qDebug() << QHostInfo::localHostName();
    qDebug() << QString("Defolter") + '@' + QHostInfo::localHostName()
           + ':' + QString::number(server.serverPort());
}
*/
bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
        return peers.contains(senderIp);

    if (!peers.contains(senderIp))
        return false;

    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections) {
        if (connection->peerPort() == senderPort)
            return true;
    }

    return false;
}

void Client::newConnection(Connection *connection)
{
    qDebug() << Q_FUNC_INFO;

    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::readyForUse()
{
    qDebug() << Q_FUNC_INFO;

    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(),
                                     connection->peerPort()))
        return;
    //when new messages is come we resend it up
    //check type and then resent to up (I SHLOUD ADD METHOD FOR CHECKING TYPE)
    /*connect(connection, SIGNAL(newMessage(QString,QString)),
            this, SIGNAL(newMessage(QString,QString)));*/

    peers.insert(connection->peerAddress(), connection);
}

void Client::getAddr()
{
    qDebug() << Q_FUNC_INFO;
    // save into file
    // send getaddr to connection()
}

void Client::connectTo(QString addresses)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << addresses;
    // token get addresses
    //
}

void Client::disconnected()
{
    qDebug() << Q_FUNC_INFO;

    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::connectionError(QAbstractSocket::SocketError)
{
    qDebug() << Q_FUNC_INFO;

    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::removeConnection(Connection *connection)
{
    qDebug() << Q_FUNC_INFO;

    if (peers.contains(connection->peerAddress())) {
        peers.remove(connection->peerAddress());
    }
    connection->deleteLater();
}
