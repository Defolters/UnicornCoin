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
        WAITING,  /*!< Waiting for version message to establish connection */
        //PREPARING,
        READY  /*!< Ready to read */
    };

    Connection(QObject *parent = 0);

    /*QString name() const;
    void setGreetingMessage(const QString &message);*/
    bool sendMessage(const QString &data);  //!< Sends data

signals:
    //! signal is emitted, when socket is ready for use
    void readyForUse();
    //! signal is emitted, when new message is recieved
    void newMessage(const MessageType type, const QString &data);

protected:
    //! I don't know the purpose of it
    //void timerEvent(QTimerEvent *timerEvent) override;

private slots:
    /*void processReadyRead();
    */
    //! Method which sends ping to check that connection is alive
    void sendPing();
    //! Method sends confirmation, that connection is established
    void sendVerack();/*
    void sendGreetingMessage();*/

private:
    /*int readDataIntoBuffer(int maxSize = MaxBufferSize);
    int dataLengthForCurrentDataType();
    bool readProtocolHeader();
    bool hasEnoughData();
    void processData();

    QString greetingMessage;
    QString username;
    QTimer pingTimer;
    QTime pongTime;
    QByteArray buffer;
    ConnectionState state;
    int numBytesForCurrentDataType;
    int transferTimerId;
    bool isGreetingMessageSent;*/
    ConnectionState state;  //!< state of connection
    MessageType currentMessageType;  //!< current type of message
};

#endif // CONNECTION_H
