#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QObject>
#include <QJsonObject>
#include <QJsonArray>

class TransactionManager : public QObject
{
    Q_OBJECT
public:
    explicit TransactionManager(QObject *parent = nullptr);

    QByteArray createNewTransaction(QList<int> inputs, QString recipient,
                                    int amount, int fee);

signals:

public slots:
};

#endif // TRANSACTIONMANAGER_H
/*
����

in � out ���� json
in:
hash of tx
index in output
script

out:
value
public key


public key + script = my money

�������� �������������, ������� � �������� ��� ������� �������.

*/
