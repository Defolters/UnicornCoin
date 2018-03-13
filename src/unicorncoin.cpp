#include "unicorncoin.h"

UnicornCoin::UnicornCoin(QObject *parent) :
    QObject(parent),
    wallet(new Wallet(this)),
    tcpsocket(nullptr),
    con(nullptr),
    txManager(nullptr)

{
    wallet->load();

    /*connect(&client, SIGNAL(newData(MessageType,QString)),
            this, SLOT(newData(MessageType,QString)));
    connect(&client, SIGNAL(newRequest(MessageType,QString,Connection*)),
            this, SLOT(newRequest(MessageType,QString,Connection*)));
    connect(&client, SIGNAL(networkPage(int)),
            this, SLOT(networkPage(int)));*/
}

void UnicornCoin::sendMessage(const QString &data)
{
    if (con != nullptr)
    {
//        con->write(data);
        con->sendMessage(MessageType::TX, data);
    }
    else qDebug() << "tcpsocket == nullptr";
}

void UnicornCoin::createNewTransaction(QString recipient, int amount, int fee)
{
    /*
        ���������, ��� ����� � ����� ����������, ����� �������� ������
        ���������, ��� � ��� �� ����� (���� unspent ������) ���������� ����� � ������������ ������ in, ����� �������� ������
        ������� �����������, �������� �� � unconfirmed � ���������� � ��� ����
    */
    //KeyGenerator::checkAddress(recipient);
    // ��������� ���� �� ����� ���������� � ������� outputs
    // ���� �����������������, �� ����������� ������
    try
    {
//        TYPE input = wallet->checkMoney(amount+fee);
        // QPair<hash, numberOfOutput>
        // list of values, which >= amont+fee
//        QList<int> listOfOutputs;
    }
    catch(...)
    {
        //s; // ������������ �����
    }
    QList<int> listOfOutputs;
    listOfOutputs.push_back(1);
    listOfOutputs.push_back(2);
    listOfOutputs.push_back(3);
    txManager.createNewTransaction(listOfOutputs, recipient, amount, fee);
    //client.sendMessage();
}

void UnicornCoin::connectToNode(const QString &ip)
{
    quint16 port = 9229;
    qDebug() << port << " " <<ip;
    QHostAddress hostadd(ip);
    con = new Connection();
    con->connectToHost(hostadd, port);
    qDebug() << con->state();
    client.newConnection(con);
}
