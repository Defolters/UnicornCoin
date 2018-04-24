#include "connection.h"

#include <QtNetwork>
#include <QCryptographicHash>
//#define DEBUG
/*static const int TransferTimeout = 30 * 1000;*/
static const int PongTimeout = 60 * 1000;  //!< If we not gets answer in pongtimeout close connection 60s
static const int PingInterval = 5 * 1000;  //!< Ping interval 5s


Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    connectionState = ConnectionState::CONNECTED;
    currentDataType = DataType::UNDEFINED;
    isVersionSend = false;

//    transferTimerId = 0;
    pingTimer.setInterval(PingInterval);

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processNewData())); // process new data, when we can
    QObject::connect(this, SIGNAL(disconnected()), &pingTimer, SLOT(stop())); //stop timer, when we disconnected
    QObject::connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing())); //from time to time send ping
    QObject::connect(this, SIGNAL(connected()),this, SLOT(sendVersion())); // when we connected to new server, send version message
}

bool Connection::sendMessage(const DataType type, const QByteArray &data)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    /*if (message.isEmpty())
        return false;*/

    // SIZE#TYPE#SIZEOFDATA#DATA#SIZEOFHASH#HASHOFDATA
    QByteArray typeM = dataTypeStr.at(static_cast<unsigned int>(type)).toUtf8();
    //QByteArray dat = data.toUtf8();
    QByteArray hash = QCryptographicHash::hash(data, QCryptographicHash::Md5).toHex();
    QByteArray raw = QByteArray::number(typeM.size()) + "#" + typeM + "#" +
                     QByteArray::number(data.size()) + "#" + data + "#" +
                     QByteArray::number(hash.size()) + "#" + hash;
#ifdef DEBUG
    qDebug() << raw;
#endif
    return write(raw) == raw.size();
}

/*
void Connection::timerEvent(QTimerEvent *timerEvent)
{
    qDebug() << Q_FUNC_INFO;
    if (timerEvent->timerId() == transferTimerId) {
        abort();
        killTimer(transferTimerId);
        transferTimerId = 0;
    }
}
*/

void Connection::processNewData()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
//    start connection -> send version -> wait for verack -> ready
//    wait for version -> send verack -> ready
    qDebug() << "My port:" << this->peerPort();
#endif
    try
    {
        readNewData();
    }
    catch(const std::exception &ex)
    {
        qDebug() << ex.what();
        //return;
    }

    if (connectionState == ConnectionState::CONNECTED)
    {
        if (isVersionSend && (currentDataType == VERACK))
        {
            //all is fine?
        }
        else if (!isVersionSend && (currentDataType == VERSION))
        {
            sendVersion();
        }
        else
        {
            qDebug() << "Not connected";
            abort();
            return;
        }
//        connectionState = ConnectionState::WAITING;
        connectionState = ConnectionState::READY;
        pingTimer.start();
        pongTime.start();

        emit readyForUse();
        return;
    }

    switch (currentDataType) {
    case UNDEFINED:
        break;
    case PING:
        sendMessage(PONG, "I am alive");
        break;
    case PONG:
        pongTime.restart();
        break;
    default:
        emit newMessage(currentDataType, buffer);
        break;
    }

    currentDataType = UNDEFINED;
    buffer.clear();
}

void Connection::sendPing()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    // Check that connection is answering to us, else abort
    if (pongTime.elapsed() > PongTimeout) {
        abort();
        return;
    }

    sendMessage(DataType::PING, "Are you alive?");
}

void Connection::readNewData()
{
    // ПЛОХО ЧИТАЕТ, ЕСЛИ ОТПРАВЛЕНО ПОРЦИЯМИ?
    // А ЧТО, ЕСЛИ ДАННЫХ МНОГО?
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    //7#Version#2#De#5#12345
    int size;

    size = readSize();

    QByteArray type = read(size);
    read(1); //read # before size
#ifdef DEBUG
    qDebug() << size << " " << type;
#endif
    size = readSize();
    buffer = read(size);
    read(1); //read # before size
#ifdef DEBUG
    qDebug() << size << " " << QString::fromUtf8(buffer);
#endif
    size = readSize();
    QByteArray md5 = read(size);
#ifdef DEBUG
    qDebug() << size << " " << md5;
#endif
    int indexOfType = dataTypeStr.indexOf(QString::fromUtf8(type));
    currentDataType = static_cast<DataType>(indexOfType);
#ifdef DEBUG
    qDebug() << indexOfType;
#endif
    //readAll();

    // check that md5 of data == md5(buffer)
    if (QCryptographicHash::hash(buffer, QCryptographicHash::Md5).toHex() == md5)
    {
#ifdef DEBUG
        qDebug() << "Hash is valid";
#endif
    }
    else
    {
        throw std::invalid_argument("Hash is NOT valid");
    }

}

int Connection::readSize()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    QByteArray size;
    bool numberSign = false;
    //что, если знака такого нет?, все сломается и будет ждать #
    //bytesAvailable() 0 if nothing or size
    while (bytesAvailable())
    {
        size.append(read(1));
        if (size.endsWith("#"))
        {
            numberSign = true;
            break;
        }
    }

    if (!numberSign)
    {
        throw std::invalid_argument("Invalid data");
    }
    size.remove(size.size()-1, 1);

    return size.toInt();
}

void Connection::sendVersion()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    DataType type;

    if (isVersionSend || (currentDataType == VERSION))
    {
        type = VERACK;
    }
    else
    {
        type = VERSION;
        isVersionSend = true;
    }

    sendMessage(type, QHostInfo::localHostName().toUtf8());
}
/*

bool Connection::readProtocolHeader()
{
    qDebug() << Q_FUNC_INFO;

    if (transferTimerId) {
        killTimer(transferTimerId);
        transferTimerId = 0;
    }

    if (readDataIntoBuffer() <= 0) {
        transferTimerId = startTimer(TransferTimeout);
        return false;
    }

bool Connection::hasEnoughData()
{
    qDebug() << Q_FUNC_INFO;

    if (transferTimerId) {
        QObject::killTimer(transferTimerId);
        transferTimerId = 0;
    }

    if (numBytesForCurrentDataType <= 0)
        numBytesForCurrentDataType = dataLengthForCurrentDataType();

    if (bytesAvailable() < numBytesForCurrentDataType
            || numBytesForCurrentDataType <= 0) {
        transferTimerId = startTimer(TransferTimeout);
        return false;
    }

    return true;
}
*/
