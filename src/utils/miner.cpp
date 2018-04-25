#include "miner.h"

#include <QCryptographicHash>
#include <QDebug>
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QString>
#include <limits>
#include <QFile>

Miner::Miner(QJsonObject block) : block(block), stopFlag(false)
{
    int difficulty = block["difficulty"].toInt();
    QString targetStr = QString(64, QChar('0'));
    targetStr.replace(difficulty, 1, "1");

    qDebug() << "Difficulty: " << difficulty << " targetStr: " << targetStr;
    target = QByteArray::fromHex(targetStr.toUtf8());
}

void Miner::run()
{
    QElapsedTimer timer;
    timer.start();
    int max = std::numeric_limits<int>::max();
    qDebug() << "Max " << max;
    QByteArray hash;

    for (int nonce = 0; ((nonce < max) && !stopFlag); nonce++)
    {
        // запихнуть нонсе
        block["nonce"] = nonce;
        // гетдок
        QJsonDocument blockDoc(block);

        hash = QCryptographicHash::hash(blockDoc.toJson(), QCryptographicHash::Sha3_256);


        // проверить хэш
        if (hash < target)
        {
            qDebug() << "Nonce found!!! Hash: " << hash.toHex() << "time: " << timer.elapsed()*0.001 << " nonce: " << block["nonce"].toInt();

            //block["hash"] = QString::fromLatin1(hash.toBase64());
            block["hash"] = QString(hash.toHex());

            QByteArray hashh = QByteArray::fromHex(block["hash"].toString().toUtf8());
            if(hash == hashh){qDebug() << "YEEEEE IT WORKS";}
            // DEBUG
            /*qDebug() << block;

            QJsonDocument doc(block);
            QFile file("block0.dat");

            if(file.open(QIODevice::WriteOnly)){
                qDebug() << "\nWriting into file..";
                QString out = doc.toJson();
                file.write(out.toUtf8());
                file.close();
            }*/

            //DEBUG
            qDebug()<<"EMITTING";
            emit newBlock(block);
            break;
        }
    }
}

void Miner::forceStop()
{
    stopFlag = true;
}

QJsonObject Miner::mineBlock(QJsonObject)
{
    QElapsedTimer timer;
    timer.start();
    int max = std::numeric_limits<int>::max();
    qDebug() << "Max " << max;
    QByteArray hash;

    for (int nonce = 0; ((nonce < max) && !stopFlag); nonce++)
    {
        // запихнуть нонсе
        block["nonce"] = nonce;
        // гетдок
        QJsonDocument blockDoc(block);

        hash = QCryptographicHash::hash(blockDoc.toJson(), QCryptographicHash::Sha3_256);


        // проверить хэш
        if (hash < target)
        {
            qDebug() << "Nonce found!!! Hash: " << hash.toHex() << "time: " << timer.elapsed()*0.001 << " nonce: " << block["nonce"].toInt();

            //block["hash"] = QString::fromLatin1(hash.toBase64());
            block["hash"] = QString(hash.toHex());

            QByteArray hashh = QByteArray::fromHex(block["hash"].toString().toUtf8());
            if(hash == hashh){qDebug() << "YEEEEE IT WORKS";}
            // DEBUG
            /*qDebug() << block;

            QJsonDocument doc(block);
            QFile file("block0.dat");

            if(file.open(QIODevice::WriteOnly)){
                qDebug() << "\nWriting into file..";
                QString out = doc.toJson();
                file.write(out.toUtf8());
                file.close();
            }*/

            //DEBUG
            return block;
        }
    }
}
