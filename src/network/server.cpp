#include <QtNetwork>

#include "connection.h"
#include "server.h"
#include "qt/mainwindow.h"

Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    //listen anyone who connects to 9229 port
    listen(QHostAddress::Any, 9229);
    //list of my local ip, which is correct?
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             qDebug() << address.toString();
    }
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    qDebug() << Q_FUNC_INFO;
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);
    emit newConnection(connection);
}
