#ifndef BLOCKMANAGER_H
#define BLOCKMANAGER_H

#include <QObject>

class BlockManager : public QObject
{
    Q_OBJECT
public:
    explicit BlockManager(QObject *parent = nullptr);

signals:

public slots:
};

#endif // BLOCKMANAGER_H