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
    qDebug() << Q_FUNC_INFO;

    QByteArray array = server_socket.read(server_socket.bytesAvailable());
    qDebug() << array;
}

void server::tcpError(QAbstractSocket::SocketError error) {
    qDebug() << Q_FUNC_INFO;

    QMessageBox::warning( (QWidget *)this->parent(), tr("Error"),tr("TCP error: %1").arg( server_socket.errorString() ) );
}

bool server::start_listen(int port_no) {
    qDebug() << Q_FUNC_INFO;

    if( !this->listen( QHostAddress::Any, port_no ) ) {
        QMessageBox::warning( (QWidget *)this->parent(), tr("Error!"), tr("Cannot listen to port %1").arg(port_no) );
    }
    else
    {
        //connect(this, SIGNAL(newConnection()), this, SLOT(new_Connection()));
        qDebug() << "Success!";
        return true;
    }
}

void server::incomingConnection(int descriptor) {
    qDebug() << Q_FUNC_INFO;

    if( !server_socket.setSocketDescriptor( descriptor ) ) {
        QMessageBox::warning( (QWidget *)this->parent(), tr("Error!"), tr("Socket error!") );
        return;
    }
    QByteArray data = "DEBUG";
    server_socket.write(data);
}
void server::new_Connection()
{
    qDebug() << Q_FUNC_INFO;

    // need to grab the socket
    QTcpSocket *socket = nextPendingConnection();

    socket->write("Hello client\r\n");
    socket->flush();

    socket->waitForBytesWritten(3000);

    socket->close();
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
