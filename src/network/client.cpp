#include <QtNetwork>

#include "client.h"
#include "connection.h"

//#define DEBUG

static const int AddrInterval = 30 * 1000;  //!< getAddr interval 30s
static const int MAX_CONNECTIONS = 40; //!< max number of connections

Client::Client()
{
    QObject::connect(&server, SIGNAL(newConnection(Connection*)),
                     this, SLOT(newConnection(Connection*)));

    // read addresses and establish initial connections
    qDebug() << QDir::currentPath();
    QFile file("addresses.dat");

    if(file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "addresses opened";
        QTextStream addrTS(&file);
        QString addresses = addrTS.readAll();
        qDebug() << file.size() << addresses;

        connectToNodes(addresses);
    }

    // set timer for a method, which sends request for new addresses, connects and saves addresses
    addrTimer.setInterval(AddrInterval);
    QObject::connect(&addrTimer, SIGNAL(timeout()), this, SLOT(getAddr()));
    addrTimer.start();

    //C:\Users\Defolter>netstat -an |find /i "9229"
    //list of my local ip, which is correct?
    foreach (const QHostAddress &address, QNetworkInterface::allAddresses()) {
        if (address.protocol() == QAbstractSocket::IPv4Protocol && address != QHostAddress(QHostAddress::LocalHost))
             qDebug() << address.toString();
    }
}

void Client::sendMessage(const MessageType &type, const QString &message)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    if (message.isEmpty())
        return;

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendMessage(type, message);
}

bool Client::hasConnection(const QHostAddress &host) const
{
    return peers.contains(host.toString());
}

void Client::connectToNodes(const QString &addresses)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    QStringList addrList = addresses.split(" ");
    QStringListIterator addrListIter(addrList);

    while (addrListIter.hasNext())
    {

        QString address = addrListIter.next();
        if (address.isEmpty())
            continue;
#ifdef DEBUG
        qDebug() << address;
#endif
        QHostAddress host(address);
        quint16 port = 9229;

        if (hasConnection(host))
            continue;

        Connection* connection = new Connection();
        connection->connectToHost(host, port);
        newConnection(connection);
    }
}

void Client::newConnection(Connection *connection)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    if (peers.size() > MAX_CONNECTIONS)
        return;
#ifdef DEBUG
    qDebug() << " " << connection->peerAddress().toString();
#endif
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::readyForUse()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress()))
        return;

    connect(connection, SIGNAL(newMessage(MessageType,QString)),
            this, SLOT(processData(MessageType,QString)));
    peers.insert(connection->peerAddress().toString(), connection);

    emit networkPage(peers.size());
}

void Client::processData(const MessageType &type, const QString &data)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    typedef MessageType MT;

    QList<MT> dataType;
    QList<MT> requestType;

    dataType << MT::TX << MT::BLOCK << MT::BCHAINSTATE << MT::NOTFOUND
             << MT::MEMPOOL << MT::UTXO << MT::REJECT;

    requestType << MT::GETTX << MT::GETBLOCK << MT::GETBCHAINSTATE
                << MT::GETMEMPOOL << MT::GETUTXO;
#ifdef DEBUG
    qDebug() << data;
#endif
    if (dataType.contains(type))
    {
        emit newData(type, data);
    }
    else if(requestType.contains(type))
    {
        if(Connection *connection = qobject_cast<Connection *>(sender()))
            emit newRequest(type, data, connection);
    }
    else if(type == MT::ADDR)
    {
        // save new addresses
        connectToNodes(data);
    }
    else if(type == MT::GETADDR)
    {
        if(Connection *connection = qobject_cast<Connection *>(sender()))
        {
            connection->sendMessage(MT::ADDR, peersToString());
        }
    }
}

void Client::connectionError(QAbstractSocket::SocketError)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::disconnected()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::getAddr()
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    QString addresses = peersToString();

    if (addresses.isEmpty())
        return;

    QFile file("addresses.dat");

    if(file.open(QFile::WriteOnly | QFile::Text))
    {
#ifdef DEBUG
        qDebug() << "addresses opened";
#endif
        file.write(addresses.toUtf8());
    }

    sendMessage(MessageType::GETADDR, "ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА");//"поделись телефончиками друзяшек");
}

void Client::removeConnection(Connection *connection)
{
#ifdef DEBUG
    qDebug() << Q_FUNC_INFO;
#endif
    if (peers.contains(connection->peerAddress().toString())) {
        peers.remove(connection->peerAddress().toString());
    }

    connection->deleteLater();
}

QString Client::peersToString()
{
    QString addresses;

    // при подключении к порту другого компьютера из сервера, то у нас ::ffff:ip
    // если мы сами подключаемся, то нет этих fff
    for (auto ip : peers.keys())
    {
#ifdef DEBUG
        qDebug() << "ip from list: "<<ip;
#endif
        // с двоеточиями создаются сервером, а без при подключении к кому-то, надо сохранять оба.
        addresses.append(ip.split(":").last() + " ");//(host.toString().split(":").last() + " ");
    }
#ifdef DEBUG
    qDebug() << addresses << endl;
#endif
    return addresses;
}
