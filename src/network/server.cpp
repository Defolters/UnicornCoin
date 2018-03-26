#include <QtNetwork>

#include "connection.h"
#include "server.h"
#include "qt/mainwindow.h"
#define DEBUG
Server::Server(QObject *parent)
    : QTcpServer(parent)
{
    listen(QHostAddress::Any, 9229);
}

void Server::incomingConnection(qintptr socketDescriptor)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    Connection *connection = new Connection(this);
    connection->setSocketDescriptor(socketDescriptor);
    emit newConnection(connection);
}
