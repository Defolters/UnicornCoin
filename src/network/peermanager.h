#ifndef PEERMANAGER_H
#define PEERMANAGER_H

#include <QTcpSocket>
/**
 * @brief The PeerManager class.
 * Asks clients for new addresses from time to time, update list of addresses
 * and makes new connections with ip addresses from this list
 * Makes new connections with other @class clientts
 */
//class Connection;
/*
class PeerManager : public QObject
{
    Q_OBJECT

public:
    PeerManager();

signals:
    //! when found new connection
//    void newConnection(Connection *connection);

private slots:

private:
    /*
    void makeNewConnections(); //! makes connections with addresses
    void readAddressesFromFile();  //!read addresses from file
    void writeAddressesIntoFile();  //!write addresses into file
    void getAddresses();  //! for client request
    void askAddresses(); //! asks for new addresses and makes connections

};*/
#endif // PEERMANAGER_H
/*

#include <QByteArray>
#include <QList>
#include <QObject>
#include <QTimer>
#include <QUdpSocket>

class Client;
class Connection;

class PeerManager : public QObject
{
    Q_OBJECT

public:
    PeerManager(Client *client);

    void setServerPort(int port);
    QByteArray userName() const;
    void startBroadcasting();
    bool isLocalHostAddress(const QHostAddress &address);

signals:
    void newConnection(Connection *connection);

private slots:
    void sendBroadcastDatagram();
    void readBroadcastDatagram();

private:
    void updateAddresses();

    Client *client;
    QList<QHostAddress> broadcastAddresses;
    QList<QHostAddress> ipAddresses;
    QUdpSocket broadcastSocket;
    QTimer broadcastTimer;
    QByteArray username;
    int serverPort;
};
*/
