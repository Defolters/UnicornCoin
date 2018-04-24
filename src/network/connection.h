#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

#include "utils/datatype.h"

//static const int MaxBufferSize = 1024000;  //!<

/**
 * \brief The Connection class connects to ports and sends/recieves data
 *
 */
class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    //! An enum DataType.
    enum class ConnectionState {
        CONNECTED, /*!< Waiting for version message to establish connection */
//        WAITING,  /*!< Waiting for verack (answer for our version message) */
        READY  /*!< Ready to read */
    };

    Connection(QObject *parent = 0);

    //!
    //! \brief Sends data to server
    //! \param type
    //! \param data
    //! \return
    //!
    bool sendMessage(const DataType type, const QByteArray &data);

signals:
    //!
    //! \brief Signal is emitted, when socket is ready for use
    //!
    void readyForUse();

    //!
    //! \brief Signal is emitted, when new message is recieved
    //! \param type
    //! \param data
    //!
    void newMessage(const DataType type, const QByteArray &data);

protected:
    //! I don't know the purpose of it
    //void timerEvent(QTimerEvent *timerEvent) override;

private slots:
    //!
    //! \brief Method handles new data from socket
    //!
    void processNewData();

    //!
    //! \brief Method which sends ping to check that connection is alive
    //!
    void sendPing();

    //!
    //! \brief Method sends confirmation that connection is established
    //!
    void sendVersion();

private:
    //!
    //! \brief Method reads data from socket
    //!
    void readNewData();

    //!
    //! \brief Method reads size of particular data
    //! \return
    //!
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
    DataType currentDataType;
};

#endif // CONNECTION_H
