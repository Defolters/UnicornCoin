#ifndef WALLET_H
#define WALLET_H
#include <QString>
//! Class saves private and public key, contain amount of money and history of transactions which connected to
//! address
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
1. Загружаем из файла в самом начале
2. Создаем новый ключ
3.
4.
5.
6.
*/
#include "utils/json.hpp"
using json = nlohmann::json;
class MainWindow;

class Wallet
{
public:
    Wallet(QString path);
    //! load information from file
    void load(MainWindow* mw);
    //! update wallet page, recieve page, and history
    void update(MainWindow* mw, QString& priv, QString& pub, QString& addr, double& amount, QString& history);
    //! update file and @ref update()
    void updateNewKeys(MainWindow *mw, QString &priv, QString &pub, QString &addr);
    bool updateFile(QString& priv, QString& pub, QString& addr, double& amount);
    bool setAmount(double& amount);
private:

    bool updateWallet(double amount, double unconfirmed);
    bool updateHistory();
    QString path;
    QString privateKey;
    QString publicKey;
    QString Address;
    double amount;
    double unconfirmed;
    json exportFile;
};

#endif // WALLET_H
//https://en.bitcoin.it/wiki/Wallet_encryption
