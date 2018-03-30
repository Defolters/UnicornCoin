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


class Wallet
{
public:
    Wallet(QObject* parent);

    //! load information from file
    void load();

    //!
    void setBalance(double balance);

    //! Method write wallet information into wallet.dat (encrypt)
    void updateFile();

    //! Method checks that we have enough money in wallet
    //! and returns list with ouputs
    QList<QJsonObject> checkMoney(double amount);

    //!
    void setKeys(QByteArray privateKey, QByteArray publicKey, QByteArray address);
    //!
    void setUnspent(QHash<QByteArray, QPair<QJsonObject, QList<int> > > unspent);

    //!
    QList<QJsonObject> getHistory() const;

    //!
    double getBalance() const;

    //!
    QByteArray getPrivateKey() const;

    //!
    QByteArray getPublicKey() const;

    //!
    QByteArray getAddress() const;

    //! Method returns true if keys are set, false otherwise
    bool isKeySet() const;

    //! Method saves data of wallet into file
    void saveWallet();

private:
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
    QHash<QByteArray, QPair<QJsonObject, QList<int> > > myUnspent;
};

#endif // WALLET_H
//https://en.bitcoin.it/wiki/Wallet_encryption
// encrypt wallet (file/data)
/*
std::string EncryptString(const char *instr, const char *passPhrase)
{
    std::string outstr;

    DefaultEncryptorWithMAC encryptor(passPhrase, new HexEncoder(new StringSink(outstr)));
    encryptor.Put((byte *)instr, strlen(instr));
    encryptor.MessageEnd();

    return outstr;
}

std::string DecryptString(const char *instr, const char *passPhrase)
{
    std::string outstr;

    HexDecoder decryptor(new DefaultDecryptorWithMAC(passPhrase, new StringSink(outstr)));
    decryptor.Put((byte *)instr, strlen(instr));
    decryptor.MessageEnd();

    return outstr;
}
https://stackoverflow.com/questions/5415752/how-to-save-string-username-password-in-encrypted-form-in-database-and-decryp
https://gist.github.com/VaTz88/750007588adf8c26a98ad428249351ba
https://stackoverflow.com/questions/2570679/serialization-with-qt
http://doc.qt.io/qt-5/qdatastream.html
http://doc.qt.io/archives/qt-4.8/datastreamformat.html
*/

