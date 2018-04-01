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
    //!
    //! \brief Signal which emitted, when we get new Connection
    //! \param connection
    //!
    void newConnection(Connection *connection);

protected:
    //!
    //! \brief Method from QTcpServer which is called when new connections are available
    //! \param socketDescriptor
    //!
    void incomingConnection(qintptr socketDescriptor) override;
};

#endif // SERVER_H
