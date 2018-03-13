#ifndef TRANSACTIONMANAGER_H
#define TRANSACTIONMANAGER_H

#include <QObject>

class TransactionManager : public QObject
{
    Q_OBJECT
public:
    explicit TransactionManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TRANSACTIONMANAGER_H