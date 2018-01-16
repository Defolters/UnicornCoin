#ifndef WALLET_H
#define WALLET_H
#include <QString>
//! Class saves private and public key, contain amount of money and history of transactions which connected to
//! address
/*!
@brief wallet class

работает с файлом wallet.dat
обновляет wallet page
contain amount of money, unconfirmed, history of transactions

*/
#include "utils/json.hpp"
using json = nlohmann::json;
class MainWindow;



class Wallet
{
public:
    Wallet(QString path);
    //! load information from file
    void load();
    //! update new p, p and a
    void update(MainWindow* mw, QString& priv, QString& pub, QString& addr);
private:
    bool updateFile();
    bool updateWallet();
    bool updateHistory();
    QString privateKey;
    QString publicKey;
    QString Address;
    json exportFile;
};

#endif // WALLET_H
//https://en.bitcoin.it/wiki/Wallet_encryption
