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

//C:\Users\Defolter>netstat -an |find /i "8333"
