#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

/**
 * @brief Class implements server, which listen for new connections
 */
class Connection;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);

signals:
    //! Signal which will call method in Client class
    void newConnection(Connection *connection);

protected:
    //! Method from QTcpServer which is called when new connections are available
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
/*
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
    void tcpError(QAbstractSocket::SocketError error );
    bool start_listen(QString ip_address, int port_no);
    void new_Connection();
protected:
    void incomingConnection( qintptr descriptor );
private:
    MainWindow* mw;
    std::list<QTcpSocket*> listOfSockets;


};
#endif // SERVER_H
//C:\Users\Defolter>netstat -an |find /i "8333"
*/
