#ifndef SERVER_H
#define SERVER_H

#include <QTcpServer>

/**
 * @brief Class implements server that listen for new connections
 */
class Connection;

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server(QObject *parent = 0);

signals:
    //! Signal which emitted, when we get new Connection
    void newConnection(Connection *connection);

protected:
    //! Method from QTcpServer which is called when new connections are available
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
