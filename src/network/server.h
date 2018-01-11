#ifndef SERVER_H
#define SERVER_H
#include <QtNetwork>
#include <QMessageBox>

class MainWindow;

class Server : public QTcpServer {
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);
    ~Server();
    QTcpSocket server_socket;
    void addUI(MainWindow *mw);
public slots:
    void tcpReady();
    void tcpError( QAbstractSocket::SocketError error );
    bool start_listen(QString ip_address, int port_no);
    void new_Connection();
protected:
    void incomingConnection( int descriptor );
private:
    MainWindow* mw;


};
#endif // SERVER_H
//C:\Users\Defolter>netstat -an |find /i "8333"
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
