#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork>
#include <QMessageBox>

class server : public QTcpServer {
    Q_OBJECT
public:
    explicit server(QObject *parent = 0);
    ~server();
    QTcpSocket server_socket;
public slots:
    void tcpReady();
    void tcpError( QAbstractSocket::SocketError error );
    bool start_listen(int port_no);
protected:
    void incomingConnection( int descriptor );
};
#endif // SERVER_H
/*

#include <QObject>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

signals:

public slots:
    void slotNewConnection();
    void slotServerRead();
    void slotClientDisconnected();
private:
    QTcpServer * mTcpServer;
    QTcpSocket * mTcpSocket;};



*/
