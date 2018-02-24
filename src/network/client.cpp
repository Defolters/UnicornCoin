#include <QtNetwork>

#include "client.h"
#include "connection.h"

static const int AddrInterval = 30 * 1000;  //!< getAddr interval 30s

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
}

void Client::sendMessage(const MessageType &type, const QString &message)
{
    qDebug() << Q_FUNC_INFO;

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
    qDebug() << Q_FUNC_INFO;

    QStringList addrList = addresses.split(" ");
    QStringListIterator addrListIter(addrList);

    while (addrListIter.hasNext())
    {

        QString address = addrListIter.next();
        if (address.isEmpty())
            continue;

        qDebug() << address;

        QHostAddress host(address);
        quint16 port = 9229;
        if (hasConnection(host))
            continue;

        //create new connection, slot connection and send version
        Connection* connection = new Connection();
        connection->connectToHost(host, port);
        newConnection(connection);
        // если подключаемся, оно само отправляет версию
        //connection->sendMessage(MessageType::VERSION, QHostInfo::localHostName() + " " +QString::number(server.serverPort()));
    }
}

void Client::newConnection(Connection *connection)
{
    qDebug() << Q_FUNC_INFO;
    qDebug() << " " << connection->peerAddress().toString();
    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::readyForUse()
{
    qDebug() << Q_FUNC_INFO;

    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress()))
        return;

    connect(connection, SIGNAL(newMessage(MessageType,QString)),
            this, SLOT(processData(MessageType,QString)));
//    peers.insert(connection->peerAddress(), connection);
    peers.insert(connection->peerAddress().toString(), connection);
    // insert in new multihash
    // change page with network in mainwindow with current state of network

    emit networkPage(peers.size());
}

void Client::processData(const MessageType &type, const QString &data)
{
    qDebug() << Q_FUNC_INFO;

    typedef MessageType MT;

    QList<MT> dataType;
    QList<MT> requestType;

    dataType << MT::TX << MT::BLOCK << MT::BCHAINSTATE << MT::NOTFOUND
             << MT::MEMPOOL << MT::UTXO << MT::REJECT;

    requestType << MT::GETTX << MT::GETBLOCK << MT::GETBCHAINSTATE
                << MT::GETMEMPOOL << MT::GETUTXO;

    if (dataType.contains(type))
    {
        qDebug() << data;
        emit newData(type, data);
    }
    else if(requestType.contains(type))
    {
        qDebug() << data;
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
    qDebug() << Q_FUNC_INFO;

    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::disconnected()
{
    qDebug() << Q_FUNC_INFO;

    if (Connection *connection = qobject_cast<Connection *>(sender()))
        removeConnection(connection);
}

void Client::getAddr()
{
    qDebug() << Q_FUNC_INFO;

    QString addresses = peersToString();

    // save into file
    QFile file("addresses.dat");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "addresses opened";
        file.write(addresses.toUtf8());
    }
    // send getaddr to connections
    sendMessage(MessageType::GETADDR, "ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА ERROR ОШИБКА");//"поделись телефончиками друзяшек");
}

void Client::removeConnection(Connection *connection)
{
    qDebug() << Q_FUNC_INFO;

    if (peers.contains(connection->peerAddress().toString())) {
        peers.remove(connection->peerAddress().toString());
    }

    connection->deleteLater();
}

QString Client::peersToString()
{
    QString addresses;
    QList<QString> list=peers.keys();

    // при подключении к порту другого компьютера из сервера, то у нас ::ffff:ip
    // если мы сами подключаемся, то нет этих fff
    for (auto ip : peers.keys())
    {
        qDebug() << "ip from list: "<<ip;
        // добавляем только соединения из вне, не добавляем соединения, созданные сервером.
        // неверный подход!!!
        // с двоеточиями создаются сервером, а без при подключении к кому-то, надо сохранять оба.
        addresses.append(ip.split(":").last() + " ");//(host.toString().split(":").last() + " ");
    }
    qDebug() << addresses << endl;
    return addresses;
}
