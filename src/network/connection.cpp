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
    connectionState = ConnectionState::CONNECTED;
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

    // SIZE#TYPE#SIZEOFDATA#DATA#SIZEOFHASH#HASHOFDATA

    QByteArray typeM = messageTypeStr.at(static_cast<unsigned int>(type)).toUtf8();
    QByteArray dat = data.toUtf8();
    QByteArray hash = QCryptographicHash::hash(dat, QCryptographicHash::Md5).toHex();
    QByteArray raw = QByteArray::number(typeM.size()) + "#" + typeM + "#" +
                     QByteArray::number(dat.size()) + "#" + dat + "#" +
                     QByteArray::number(hash.size()) + "#" + hash;
    qDebug() << raw;

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

start connection -> send version -> wait for verack -> ready
wait for version -> send verack -> ready
*/

void Connection::processNewData()
{
    qDebug() << "My port:" << this->peerPort();

    try
    {
        readNewData();
    }
    catch(const std::exception &ex)
    {
        qDebug() << ex.what();
    }

    if (connectionState == ConnectionState::CONNECTED)
    {
        if (isVersionSend && (currentMessageType == VERACK))
        {
            //all is fine?
        }
        else if (!isVersionSend && (currentMessageType == VERSION))
        {
            sendVersion(MessageType::VERACK);
        }
        else
        {
            qDebug() << "Not connected";
            abort();
            return;
        }

        connectionState = ConnectionState::WAITING;
        connectionState = ConnectionState::READY;
        /*pingTimer.start();
        pongTime.start();*/

        emit readyForUse();
        return;
    }

    /*if (bytesAvailable()){
        readNewData();
    }*/

    switch (currentDataType) {
    case UNDEFINED:
        break;
    case PING:
        sendMessage(PONG,"pong");
        break;
    case PONG:
        pongTime.restart();
        break;
    default:
        emit newMessage(currentMessageType, QString::fromUtf8(buffer));
        break;
    }

    currentDataType = MessageType::UNDEFINED;
    buffer.clear();
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

    sendMessage(MessageType::PING, "Ты еще не умер, бро?");
}

void Connection::readNewData()
{
    // ПЛОХО ЧИТАЕТ, ЕСЛИ ОТПРАВЛЕНО ПОРЦИЯМИ?
    qDebug() << Q_FUNC_INFO;
    //7#Version#2#De#5#12345
    int size;

    size = readSize();

    QByteArray type = read(size);
    read(1); //read # before size
    //qDebug() << size << " " << QString::fromUtf8(type);
    qDebug() << size << " " << type;

    size = readSize();
    buffer = read(size);
    read(1); //read # before size
    qDebug() << size << " " << QString::fromUtf8(buffer);

    size = readSize();
    QByteArray md5 = read(size);
    qDebug() << size << " " << md5;

    //translate type into currentMessageType
    // как лучше сохранять и обрабатывать?.. Мозжет, лучше enum instead of enum class?
    int indexOfType = messageTypeStr.indexOf(QString::fromUtf8(type));
    currentMessageType = static_cast<MessageType>(indexOfType);
    qDebug() << indexOfType;
    readAll();

    // check that md5 == md5(buffer)
    if (QCryptographicHash::hash(buffer, QCryptographicHash::Md5).toHex() == md5)
    {
        qDebug() << "Hash is valid";
    }
    else
    {
        throw std::invalid_argument("Hash is NOT valid");
    }

}

int Connection::readSize()
{
    qDebug() << Q_FUNC_INFO;

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

void Connection::sendVersion(MessageType type)
{
    qDebug() << Q_FUNC_INFO;

    sendMessage(type, QHostInfo::localHostName());

    isVersionSend = true;
}

void Connection::sendVersion()
{
    sendVersion(MessageType::VERSION);
}
/*
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
