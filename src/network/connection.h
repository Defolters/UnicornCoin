#ifndef CONNECTION_H
#define CONNECTION_H

#include <QHostAddress>
#include <QString>
#include <QTcpSocket>
#include <QTime>
#include <QTimer>

//static const int MaxBufferSize = 1024000;  //!<

/**
 * @brief The Client class
 */
class Connection : public QTcpSocket
{
    Q_OBJECT

public:
    /*enum ConnectionState {
        WaitingForGreeting,
        ReadingGreeting,
        ReadyForUse
    };
    enum DataType {
        PlainText,
        Ping,
        Pong,
        Greeting,
        Undefined
    };*/

    Connection(QObject *parent = 0);

    /*QString name() const;
    void setGreetingMessage(const QString &message);
    bool sendMessage(const QString &message);*/

signals:
    void readyForUse();  //!< signal is emitted, when socket is ready for use
    //void newData(const DataType type, const QString &data);  //!< signal is emitted, when new data is read
    /*
    void newMessage(const QString &from, const QString &message);*/

protected:
    //void timerEvent(QTimerEvent *timerEvent) override;

private slots:
    /*void processReadyRead();
    void sendPing();
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
    DataType currentDataType;
    int numBytesForCurrentDataType;
    int transferTimerId;
    bool isGreetingMessageSent;*/
};

#endif // CONNECTION_H
