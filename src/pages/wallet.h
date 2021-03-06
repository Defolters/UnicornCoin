#ifndef WALLET_H
#define WALLET_H
#include <QString>
#include <QObject>
#include <QJsonObject>
#include <QPair>
#include <QHash>
#include <QList>

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

/**
 * @brief The Wallet class implements wallet and method for working with it
 *
 */
class Wallet
{
public:
    Wallet(QObject* parent);

    //!
    //! \brief Method saves data into file
    //! Method write wallet information into wallet.dat (encrypt)
    //!
    void save();

    //!
    //! \brief Method loads data from file
    //!
    void load();

    //!
    //! \brief setBalance
    //! \param balance
    //!
    void setBalance(double balance);

    //!
    //! \brief Method checks that we have enough money in wallet and returns list with ouputs
    //! \param amount
    //! \return
    //!
    QList<QJsonObject> checkMoney(double amount);

    //!
    //! \brief Method sets keys for wallet
    //! \param privateKey
    //! \param publicKey
    //! \param address
    //!
    void setKeys(QByteArray privateKey, QByteArray publicKey, QByteArray address);

    //!
    //! \brief setUnspent
    //! \param unspent
    //!
    void setUnspent(QHash<QString, QPair<QJsonObject, QList<int> > > unspent);

    //!
    //! \brief getHistory
    //! \return
    //!
    QList<QJsonObject> getHistory() const;

    //!
    //! \brief getBalance
    //! \return
    //!
    double getBalance() const;

    double getUnconfirmed() const;

    //!
    //! \brief getPrivateKey
    //! \return
    //!
    QByteArray getPrivateKey() const;

    //!
    //! \brief getPublicKey
    //! \return
    //!
    QByteArray getPublicKey() const;

    //!
    //! \brief getAddress
    //! \return
    //!
    QByteArray getAddress() const;

    //!
    //! \brief Method returns true if keys are set, false otherwise
    //! \return
    //!
    bool isKeySet() const;

private:
    //!
    //! \brief updateHistory
    //! \return
    //!
    bool updateHistory();

    //! Status of keys?
    bool keySet;

    //!
    QString path;

    //!
    QByteArray privateKey;

    //!
    QByteArray publicKey;

    //!
    QByteArray address;

    //!
    double balance;

    //!
    double unconfirmed;

    //! History = all tx spent by me (confirmed)
    QList<QJsonObject> history;

    //!
    QHash<QString, QPair<QJsonObject, QList<int> > > myUnspent;
};

#endif // WALLET_H
