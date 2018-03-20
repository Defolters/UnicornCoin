#ifndef WALLET_H
#define WALLET_H
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QPair>
#include <QHash>
#include <QList>

//! Class saves private and public key, contain amount of money and history of transactions which connected to
//! отвечает за wallet page, recieve page and history page
/*!
@brief wallet class
загружает из файла ключи, количество денег, историю
изменяем историю
изменяем количество денег
изменяем адрес
работает с файлом wallet.dat
обновляет wallet page
contain amount of money, unconfirmed, history of transactions??
СПОСОБЫ ИЗМЕНЕНИЯ:
1. Загружаем из файла в самом начале (load keys, amount, unconfirmed and history)
2. Создаем новый ключ (new file, empty history and 0.0 unconf and amount, then ask blockObserver to give us amount and hsitory)
3. создаем транзакцию (amount -> unconfirmed)
4. получаем блоки (from god -> amount and history + unconfirmed -> disappear)
5. создаем блоки (from mining -> amount and history + like получаем блоки)
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
    QList<QJsonObject> checkMoney(double amount);

    void setKeys(QByteArray privateKey, QByteArray publicKey, QByteArray address);
    void setUnspent(QHash<QByteArray, QPair<QJsonObject, QList<int> > > &unspent);

    QList<QJsonObject> getHistory() const;
    double getBalance() const;
    QByteArray getPrivateKey() const;
    QByteArray getPublicKey() const;
    QByteArray getAddress() const;

    //! Method return true if keys are set, false otherwise
    bool isKeySet() const;

private:

    bool updateHistory();

    //! Status of keys?
    bool keySet;
    QString path;
    QByteArray privateKey;
    QByteArray publicKey;
    QByteArray address;

    double balance;
    double unconfirmed;

    //! History = all tx spent by me (confirmed)
    QList<QJsonObject> history;
    QHash<QByteArray, QPair<QJsonObject, QList<int> > > myUnspent;
};

#endif // WALLET_H
//https://en.bitcoin.it/wiki/Wallet_encryption
