#include "keygenerator.h"
#include <QDebug>
KeyGenerator::KeyGenerator()
{

}

QString KeyGenerator::generatePrivateKey()
{
    // randomly generate private key and then..
    return QString("5KYZdUEo39z3FPrtuX2QbbwGnNP5zTd7yyr2SC1j299sBCnWjss"); //len = 51
}

QString KeyGenerator::generatePrivateKey(QString words)
{
/* truePrivateKey = sha256(words);
 * checksum = first 4 bytes of sha256(sha256(truePrivateKey))
 * truePrivateAndChecksum = truePrivateKey + checksum
 * privateKey = base58(truePrivateAndChecksum)
*/
    QString s = "hello world";
    /*QByteArray hash = QCryptographicHash::hash(s.toLocal8Bit(), QCryptographicHash::Sha256);
    qDebug() << QString::fromUtf8(hash);*/
    qDebug() << Q_FUNC_INFO;
    qDebug() << QString(QCryptographicHash::hash(s.toUtf8(),QCryptographicHash::Sha256).toHex());
    return QString("5KYZdUEo39z3FPrtuX2QbbwGnNP5zTd7yyr2SC1j299sBCnWjss");
}

QString KeyGenerator::generatePublicKey(QString privateKey)
{
    /*
     * x, y = privateKey * eleptic curve
     * flag = 01 if y is odd and 02 if y is even
     * publicKey = flag + x*/
    // len = 130
    return QString("04a34b99f22c790c4e36b2b3c2c35a36db06226e41c692fc82b8b56ac1c540c5bd5b8dec5235a0fa8722476c7709c02559e3aa73aa03918ba2d492eea75abea235");
}

QString KeyGenerator::generateAddress(QString publicKey)
{
    /*
     * hashOfPublic = sha256(publicKey)
     * dobleHashOfPublic = ripe160(hashOfPublic)
     * checksum = sha256(sha256(dobleHashOfPublic))
     * doubleHashOfPublicAndChecksum = dobleHashOfPublic + checksum
     * address = base58(doubleHashOfPublicAndChecksum)*/
    return QString("1HZwkjkeaoZfTSaJxDw6aKkxp45agDiEzN"); // len = 34
}
