#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

#include "utils/messagetype.h"

//static const int MaxBufferSize = 1024000;  //!<

/**
 * @brief The Client class
 */
class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    //! An enum MessageType.
    enum class ConnectionState {
        CONNECTED, /*!< Waiting for version message to establish connection */
//        WAITING,  /*!< Waiting for verack (answer for our version message) */
        READY  /*!< Ready to read */
    };

    Connection(QObject *parent = 0);

    //!< Sends data
    bool sendMessage(const MessageType type, const QString &data);

signals:
    //! Signal is emitted, when socket is ready for use
    void readyForUse();
    //! Signal is emitted, when new message is recieved
    void newMessage(const MessageType type, const QString &data);

protected:
    //! I don't know the purpose of it
    //void timerEvent(QTimerEvent *timerEvent) override;

private slots:
    //! Method handles new data from socket
    void processNewData();
    //! Method which sends ping to check that connection is alive
    void sendPing();
    //! Method sends confirmation that connection is established
    void sendVersion();

private:
    //! Method reads data from socket
    void readNewData();
    //! Method reads size of particular data
    int readSize();
    //! Timer for sendPing()
    QTimer pingTimer;
    //! Counts time for answer to sendPing()
    QTime pongTime;
    //! Buffer for recieved data
    QByteArray buffer;
//    int transferTimerId;
    //! ...
    bool isVersionSend;
    //! State of connection
    ConnectionState connectionState;
    //! Type of recieved data
    MessageType currentDataType;
};

#endif // CONNECTION_H
