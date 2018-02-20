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
        qDebug() << file.size()<< addresses ;//<< addresses.readAll();
        //прочитать текст и отправить в функцию
        connectTo(addresses);
    }

    addrTimer.setInterval(AddrInterval);
    // set timer for a method, which sends request for new addresses and saves addresses
    // через время запрашивать новые адреса и пытаться подключиться, затем текущие соединения сохранить в файл
    QObject::connect(&addrTimer, SIGNAL(timeout()), this, SLOT(getAddr()));
    addrTimer.start();
}

void Client::sendMessage(const MessageType &type, const QString &message)
{
    qDebug() << Q_FUNC_INFO;

    /*if (message.isEmpty())
        return;*/

    QList<Connection *> connections = peers.values();
    foreach (Connection *connection, connections)
        connection->sendMessage(type, message);
}

bool Client::hasConnection(const QHostAddress &senderIp, int senderPort) const
{
    if (senderPort == -1)
        return peers.contains(senderIp);

    if (!peers.contains(senderIp))
        return false;

    QList<Connection *> connections = peers.values(senderIp);
    foreach (Connection *connection, connections) {
        if (connection->peerPort() == senderPort)
            return true;
    }

    return false;
}

void Client::connectTo(QString &addresses)
{
    qDebug() << Q_FUNC_INFO;

    QStringList addrList = addresses.split(" ");
    QStringListIterator addrListIter(addrList);

    while (addrListIter.hasNext())
    {

        QString address = addrListIter.next();
        if (address.isEmpty())
            continue;

        qDebug() << address ;//<< " " <<address.split(" ").at(0)<<""<<address.split(" ").at(1).toUInt()<<endl;

        QHostAddress host(address);
        quint16 port = 9229;//address.split(" ").at(1).toUInt();
        if (hasConnection(host, port))
            continue;

        //create new connection, slot connection and send version
        Connection* connection = new Connection();
        connection->connectToHost(host, port);
        newConnection(connection);
        //connection->sendMessage(MessageType::VERSION, QHostInfo::localHostName() + " " +QString::number(server.serverPort()));
    }
}

void Client::newConnection(Connection *connection)
{
    qDebug() << Q_FUNC_INFO;

    connect(connection, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(connectionError(QAbstractSocket::SocketError)));
    connect(connection, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(connection, SIGNAL(readyForUse()), this, SLOT(readyForUse()));
}

void Client::readyForUse()
{
    qDebug() << Q_FUNC_INFO;

    Connection *connection = qobject_cast<Connection *>(sender());
    if (!connection || hasConnection(connection->peerAddress(),
                                     connection->peerPort()))
        return;

    connect(connection, SIGNAL(newMessage(MessageType,QString)),
            this, SLOT(processData(MessageType,QString)));
    peers.insert(connection->peerAddress(), connection);
}

void Client::processData(const MessageType &type, const QString &data)
{
    qDebug() << Q_FUNC_INFO;

    typedef MessageType MT;

    QList<MT> dataType;
    QList<MT> requestType;

    dataType << MT::VERSION << MT::VERACK << MT::TX <<
                MT::BLOCK << MT::ADDR << MT::BCHAINSTATE <<
                MT::NOTFOUND << MT::MEMPOOL << MT::UTXO << MT::REJECT;

    requestType << MT::GETTX << MT::GETBLOCK << MT::GETADDR <<
                   MT::GETBCHAINSTATE << MT::GETMEMPOOL << MT::GETUTXO;

    if (dataType.contains(type))
    {
        emit newData(type, data);
    }
    else if(requestType.contains(type))
    {
        if(Connection *connection = qobject_cast<Connection *>(sender()))
            emit newRequest(type, data, connection);
    }

    switch (type)
    {
        case MT::REJECT: "nooo, what should I do now?";
    }
    //определить тип, что-то выполнить, остальное выслать наверх
    // if type == addr
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

    QString addresses;
    QList<QHostAddress> list=peers.keys();

    if (peers.empty())
    {
        qDebug() << "empty";
        return;
    }
    //there is can be an error, i didn't check it yet
    // iterate through multihash and create string with ip and port
    for (auto host : list)
    {
        QList<Connection *> values = peers.values(host);
        for (int i = 0; i < values.size(); ++i)
        {
            //"::ffff:192.168.0.102" 54823
            try{addresses.append(host.toString().split(":").at(3) + " ");}
            catch(const std::exception &ex)
            {
                qDebug() << "Exception";
            }
//            values.at(i)->localAddress();
            // подключаться к порту сокета - плохая идея. Мы не слушаем этот порт,
//            значит, нужно сохранять просто адреса?
            //addresses.append(QString::number(values.at(i)->peerPort()) + "\n");
            qDebug() << addresses << endl;
        }
    }

    // save into file
    QFile file("addresses.dat");
    if(file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "addresses opened";
        file.write(addresses.toUtf8());
    }
    // send getaddr to connections
    sendMessage(MessageType::GETADDR, "поделись телефончиками друзяшек");

    // change page with network in mainwindow with current state of network
    //emit ..
}

void Client::removeConnection(Connection *connection)
{
    qDebug() << Q_FUNC_INFO;

    if (peers.contains(connection->peerAddress())) {
        peers.remove(connection->peerAddress());
    }
    connection->deleteLater();
}
