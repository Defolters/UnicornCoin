#include "connection.h"

#include <QtNetwork>
#include <QCryptographicHash>

/*static const int TransferTimeout = 30 * 1000;*/
static const int PongTimeout = 60 * 1000;  //!< If we not gets answer in pongtimeout close connection 60s
static const int PingInterval = 5 * 1000;  //!< Ping interval 5s
/*
static const char SeparatorToken = ' ';*/

Connection::Connection(QObject *parent)
    : QTcpSocket(parent)
{
    state = ConnectionState::WAITING;
    currentMessageType = MessageType::UNDEFINED;
    isVersionSend = false;
    /*greetingMessage = tr("undefined");
    username = tr("unknown");

    numBytesForCurrentDataType = -1;
    transferTimerId = 0;
    isGreetingMessageSent = false;*/
    pingTimer.setInterval(PingInterval);

    QObject::connect(this, SIGNAL(readyRead()), this, SLOT(processNewData())); // process new data, when we can
    QObject::connect(this, SIGNAL(disconnected()), &pingTimer, SLOT(stop())); //stop timer, when we disconnected
    QObject::connect(&pingTimer, SIGNAL(timeout()), this, SLOT(sendPing())); //from time to time send ping
    QObject::connect(this, SIGNAL(connected()),this, SLOT(sendVersion())); // when we connected to new server, send version message
}

bool Connection::sendMessage(const MessageType type, const QString &data)
{
    qDebug() << Q_FUNC_INFO;
    /*if (message.isEmpty())
        return false;*/
    // TYPE#SIZEOFDATA#DATA#HASHOFDATA
    QByteArray dat = data.toUtf8();
    QByteArray raw = messageTypeStr.at(static_cast<unsigned int>(type)).toUtf8()
            + "#" +
            QByteArray::number(dat.size()) + "#" +dat.toHex()
            + "#" +
            QCryptographicHash::hash(dat, QCryptographicHash::Md5).toHex();
            //QString(QCryptographicHash::hash(s.toUtf8(),QCryptographicHash::Sha256).toHex())
    qDebug() << raw;
//    qDebug() << QString::fromUtf8(raw.split('#').at(2)); // а если нет этого?..
    //QByteArray msg = data.toUtf8();
    //QByteArray data1 = messageTypeStr.at(static_cast<unsigned int>(type)).toUtf8() +"#"+ QByteArray::number(dat.size()) + ' ' + dat;
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
    emit readyForUse();

    try
    {
        readNewData();
    }
    catch(std::)

    if (state == ConnectionState::CONNECTED)
    {
        //check that we got version
        //send verack in response
    }
    if (state == ConnectionState::WAITING)
    {}
    //MessageType::
    //split new data to type and Qstring data
    /*
    qDebug() << Q_FUNC_INFO;

    if (state == WaitingForGreeting) {
        if (!readProtocolHeader())
            return;
        if (currentDataType != Greeting) {
            abort();
            return;
        }
        state = ReadingGreeting;
    }

    if (state == ReadingGreeting) {
        if (!hasEnoughData())
            return;

        buffer = read(numBytesForCurrentDataType);
        qDebug() <<"serverarray " << buffer;
        if (buffer.size() != numBytesForCurrentDataType) {
            abort();
            return;
        }

        username = QString(buffer) + '@' + peerAddress().toString() + ':'
                   + QString::number(peerPort());
        currentDataType = Undefined;
        numBytesForCurrentDataType = 0;
        buffer.clear();

        if (!isValid()) {
            abort();
            return;
        }

        if (!isGreetingMessageSent)
            sendGreetingMessage();

        pingTimer.start();
        pongTime.start();
        state = ReadyForUse;
        emit readyForUse();
    }

    do {
        if (currentDataType == Undefined) {
            if (!readProtocolHeader())
                return;
        }
        if (!hasEnoughData())
            return;
        processData();
    } while (bytesAvailable() > 0);*/
}

void Connection::sendPing()
{
    qDebug() << Q_FUNC_INFO;

    // Check that connection is answering to us, else abort
    if (pongTime.elapsed() > PongTimeout) {
        abort();
        return;
    }
    sendMessage(MessageType::PING, "“ы еще не умер, бро?");
    //write("PING 1 p");
}

void Connection::readNewData()
{
    qDebug() << Q_FUNC_INFO;
    int size;

    size = readSize();
    QByteArray type = read(size);

    size = readSize();
    buffer = read(size);

    size = readSize();
    QByteArray md5 = read(size);

    //translate type into currentMessageType
    // как лучше сохран€ть и обрабатывать?.. ћозжет, лучше enum instead of enum class?
    int indexOfType = messageTypeStr.indexOf(QString::fromUtf8(type));

    // check that md5 == md5(buffer)
    //else throw error

    //читаем до #, затем обрабатываем число, затем читаем решетку,
    //    затем читаем до решетки, затем то же самое.
}

int Connection::readSize()
{
    qDebug() << Q_FUNC_INFO;

    QByteArray size;

    while (!size.endsWith("#"))
    {
        size.append(read(1));
    }
    size.remove(size.size()-1, 1);

    return size.toInt();
}

void Connection::sendVersion(MessageType type)
{
    qDebug() << Q_FUNC_INFO;

    //send version
    sendMessage(type, QHostInfo::localHostName());
    /*if(type==MessageType::VERACK)
    {
        state = ConnectionState::READY;
    }*/
    isVersionSend = true;
}
/*
void Connection::sendGreetingMessage()
{
    qDebug() << Q_FUNC_INFO;
    QByteArray greeting = greetingMessage.toUtf8();
    QByteArray data = "GREETING " + QByteArray::number(greeting.size()) + ' ' + greeting;
    if (write(data) == data.size())
        isGreetingMessageSent = true;
}

int Connection::readDataIntoBuffer(int maxSize)
{
    qDebug() << Q_FUNC_INFO;

    if (maxSize > MaxBufferSize)
        return 0;

    int numBytesBeforeRead = buffer.size();
    if (numBytesBeforeRead == MaxBufferSize) {
        abort();
        return 0;
    }

    while (bytesAvailable() > 0 && buffer.size() < maxSize) {
        buffer.append(read(1));
        if (buffer.endsWith(SeparatorToken))
            break;
    }
    qDebug() <<"readDataIntoBuffer " << buffer;
    return buffer.size() - numBytesBeforeRead;
}

int Connection::dataLengthForCurrentDataType()
{
    if (bytesAvailable() <= 0 || readDataIntoBuffer() <= 0
            || !buffer.endsWith(SeparatorToken))
        return 0;

    buffer.chop(1);
    int number = buffer.toInt();
    buffer.clear();
    return number;
}

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

    if (buffer == "PING ") {
        currentDataType = Ping;
    } else if (buffer == "PONG ") {
        currentDataType = Pong;
    } else if (buffer == "MESSAGE ") {
        currentDataType = PlainText;
    } else if (buffer == "GREETING ") {
        currentDataType = Greeting;
    } else {
        currentDataType = Undefined;
        abort();
        return false;
    }

    buffer.clear();
    numBytesForCurrentDataType = dataLengthForCurrentDataType();
    return true;
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

void Connection::processData()
{
    qDebug() << Q_FUNC_INFO;

    buffer = read(numBytesForCurrentDataType);
    if (buffer.size() != numBytesForCurrentDataType) {
        abort();
        return;
    }

    switch (currentDataType) {
    case PlainText:
        emit newMessage(username, QString::fromUtf8(buffer));
        break;
    case Ping:
        write("PONG 1 p");
        break;
    case Pong:
        pongTime.restart();
        break;
    default:
        break;
    }

    currentDataType = Undefined;
    numBytesForCurrentDataType = 0;
    buffer.clear();
}
*/
