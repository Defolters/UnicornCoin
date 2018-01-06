#include "Server.h"

server::server(QObject *parent) : QTcpServer(parent) {
    connect( &server_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpError(QAbstractSocket::SocketError)) );
    connect( &server_socket, SIGNAL(readyRead()),
             this, SLOT(tcpReady()) );
    server_socket.setSocketOption(QAbstractSocket::KeepAliveOption, true );
}

server::~server() {
    server_socket.disconnectFromHost();
    server_socket.waitForDisconnected();
}

void server::tcpReady() {
    QByteArray array = server_socket.read(erver_socket.bytesAvailable());
}

void server::tcpError(QAbstractSocket::SocketError error) {
    QMessageBox::warning( (QWidget *)this->parent(), tr("Error"),tr("TCP error: %1").arg( server_socket.errorString() ) );
}

bool server::start_listen(int port_no) {
    if( !this->listen( QHostAddress::Any, port_no ) ) {
        QMessageBox::warning( (QWidget *)this->parent(), tr("Error!"), tr("Cannot listen to port %1").arg(port_no) );
    }
    else
        return true;
}

void server::incomingConnection(int descriptor) {
    if( !server_socket.setSocketDescriptor( descriptor ) ) {
        QMessageBox::warning( (QWidget *)this->parent(), tr("Error!"), tr("Socket error!") );
        return;
    }
}
/*#include <QDebug>
#include <QCoreApplication>

Server::Server(QObject *parent) : QObject(parent)
{
    mTcpServer = new QTcpServer(this);

    connect(mTcpServer, &QTcpServer::newConnection, this, &Server::slotNewConnection);

    if(!mTcpServer->listen(QHostAddress::Any, 6000)){
        qDebug() << "server is not started";
    } else {
        qDebug() << "server is started";
    }
}

void Server::slotNewConnection()
{
    mTcpSocket = mTcpServer->nextPendingConnection();

    mTcpSocket->write("Hello, World!!! I am echo server!\r\n");

    connect(mTcpSocket, &QTcpSocket::readyRead, this, &Server::slotServerRead);
    connect(mTcpSocket, &QTcpSocket::disconnected, this, &Server::slotClientDisconnected);
}

void Server::slotServerRead()
{
    while(mTcpSocket->bytesAvailable()>0)
    {
        QByteArray array = mTcpSocket->readAll();

        mTcpSocket->write(array);
    }
}

void Server::slotClientDisconnected()
{
    mTcpSocket->close();
}
*/
