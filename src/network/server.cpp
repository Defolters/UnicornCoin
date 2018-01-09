#include "server.h"
#include "qt/mainwindow.h"
Server::Server(QObject *parent) : QTcpServer(parent) {
    connect( &server_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpError(QAbstractSocket::SocketError)) );
    connect( &server_socket, SIGNAL(readyRead()),
             this, SLOT(tcpReady()) );
    server_socket.setSocketOption(QAbstractSocket::KeepAliveOption, true );
}

Server::~Server() {
    server_socket.disconnectFromHost();
    server_socket.waitForDisconnected();
}

void Server::addUI(MainWindow *mw)
{
    qDebug() << Q_FUNC_INFO;
    this->mw = mw;
}

void Server::tcpReady() {
    qDebug() << Q_FUNC_INFO;

    QByteArray array = server_socket.read(server_socket.bytesAvailable());
    qDebug() <<"serverarray " <<array;
    QString str;
    str = QString::fromUtf8(array);
    //str = "str";
    mw->change_data(str, "label_3");
    /**/

}

void Server::tcpError(QAbstractSocket::SocketError error) {
    qDebug() << Q_FUNC_INFO;

    QMessageBox::warning( (QWidget *)this->parent(), tr("Error"),tr("TCP error: %1").arg( server_socket.errorString() ) );
}

bool Server::start_listen(int port_no) {
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

void Server::incomingConnection(int descriptor) {
    qDebug() << Q_FUNC_INFO;

    if( !server_socket.setSocketDescriptor( descriptor ) ) {
        QMessageBox::warning( (QWidget *)this->parent(), tr("Error!"), tr("Socket error!") );
        return;
    }
    QByteArray data = "DEBUG";
    server_socket.write(data);
    qDebug() << server_socket.peerAddress();
    qDebug() << server_socket.peerPort();
    mw->change_data(server_socket.peerAddress().toString(), "ip_con");
    mw->change_data(QString::number(server_socket.peerPort()), "port_con");


}
void Server::new_Connection()
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
