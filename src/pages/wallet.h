#ifndef WALLET_H
#define WALLET_H
#include <QString>
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
#include "utils/json.hpp"
using json = nlohmann::json;
class MainWindow;

class Wallet
{
public:
    Wallet(MainWindow* parent);
    //! load information from file
    void load();
    //! update wallet page, recieve page, and history
    void update(MainWindow* mw, QString& priv, QString& pub, QString& addr, double amount, QString& history);
    //! update file and @ref update()
    void updateNewKeys(MainWindow *mw, QString &priv, QString &pub, QString &addr);
    bool updateFile(QString& priv, QString& pub, QString& addr, double amount);
    bool setAmount(double& amount);
    json getHistory();
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
    json history;
    MainWindow* parent;
};

#endif // WALLET_H
//https://en.bitcoin.it/wiki/Wallet_encryption
