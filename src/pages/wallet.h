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

