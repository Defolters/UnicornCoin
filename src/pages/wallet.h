#ifndef WALLET_H
#define WALLET_H
#include <QString>
#include <QObject>
#include <QJsonObject>

//! Class saves private and public key, contain amount of money and history of transactions which connected to
//! �������� �� wallet page, recieve page and history page
/*!
@brief wallet class
��������� �� ����� �����, ���������� �����, �������
�������� �������
�������� ���������� �����
�������� �����
�������� � ������ wallet.dat
��������� wallet page
contain amount of money, unconfirmed, history of transactions??
������� ���������:
1. ��������� �� ����� � ����� ������ (load keys, amount, unconfirmed and history)
2. ������� ����� ���� (new file, empty history and 0.0 unconf and amount, then ask blockObserver to give us amount and hsitory)
3. ������� ���������� (amount -> unconfirmed)
4. �������� ����� (from god -> amount and history + unconfirmed -> disappear)
5. ������� ����� (from mining -> amount and history + like �������� �����)
6.
*/


class Wallet
{
public:
    Wallet(QObject* parent);

    //! load information from file
    void load();

    void setBalance(double balance);

    //! Method write wallet information into wallet.dat (encrypt)
    void updateFile();

    //! Method checks that we have enough money in wallet
    //! and returns list with ouputs
    QList<double> checkMoney(double amount);

    void setKeys(QByteArray privateKey, QByteArray publicKey, QByteArray address);
    void setUnspent(QList<QJsonObject> &unspent);

    QList<QJsonObject> getHistory() const;
    double getBalance() const;
    QByteArray getPrivateKey() const;
    QByteArray getPublicKey() const;
    QByteArray getAddress() const;

private:

    bool updateHistory();
    QString path;
    QByteArray privateKey;
    QByteArray publicKey;
    QByteArray address;

    double balance;
    double unconfirmed;

    //! History = all tx spent by me (confirmed)
    QList<QJsonObject> history;
    QList<QJsonObject> myUnspent;
//    MainWindow* parent;
};

#endif // WALLET_H
//https://en.bitcoin.it/wiki/Wallet_encryption
