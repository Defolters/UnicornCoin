#include "transactionmanager.h"
#include <QDebug>
#include <QJsonDocument>
#include <cryptlib.h>
#include <osrng.h>
#include <eccrypto.h>
#include <oids.h>

QByteArray TransactionManager::createNewTransaction(QList<double> inputs,
                                                    QByteArray recipient,
                                                    QByteArray privateKey,
                                                    QByteArray publicKey,
                                                    double amount, double fee)
{
    QJsonObject tx;
    tx["type"] = 1;
    tx["fee"] = fee;

    qDebug() << tx;

    QJsonArray in;

    for (int i =0;i<inputs.size();i++)
    {
        QJsonObject input;
        input["value"] = inputs.at(i);
        input["pubkey"] = QString(publicKey.toHex());

        in << input;
    }
    // in добавить потом, проверить, что хэш до и после добавления одинаковый
    tx["in"] = in;

    QJsonArray out;
    QJsonObject output;
    output["value"] = amount;
    output["recipient"] = QString::fromUtf8(recipient);
    // if sum of inputs > amount + fee
    // add in output

    QJsonDocument txJD(tx);

    tx["signature"] = QString(signTransaction(txJD.toJson(), privateKey).toHex());

    qDebug() << tx;

    if (verifyTransaction(tx))
    {
        qDebug() << "yee";
    }
    else{qDebug()<<"NOOOO!!";}

    return QByteArray();
}

QByteArray TransactionManager::signTransaction(QByteArray tx, QByteArray privateKeyData)
{
    qDebug () << "private key for signing:" << privateKeyData.toHex();
    CryptoPP::AutoSeededRandomPool autoSeededRandomPool;

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
    privateKey.Initialize(autoSeededRandomPool, CryptoPP::ASN1::secp160r1());

    CryptoPP::Integer privateKeyInteger = 0;
    privateKeyInteger.Decode((CryptoPP::byte*)privateKeyData.data(), (size_t)privateKeyData.size());
    privateKey.SetPrivateExponent(privateKeyInteger);

    bool result = privateKey.Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "private key is not valid!";
        return QByteArray();
    }

    QByteArray message = tx;
    qDebug() << "transaction message:" << message;

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Signer signer(privateKey);

    result = signer.AccessKey().Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "failed to create signer" << result;
        return QByteArray();
    }

    // Determine maximum size, allocate a string with the maximum size
    size_t siglen = signer.MaxSignatureLength();
    QByteArray signature(siglen, 0x00);


    // Sign, and trim signature to actual size
    siglen = signer.SignMessage( autoSeededRandomPool, (const CryptoPP::byte*)message.data(), message.size(), (CryptoPP::byte*)signature.data() );

    QByteArray m_signature = QByteArray(signature.data(), siglen);

    return QByteArray();
}

bool TransactionManager::verifyTransaction(QJsonObject tx)
{
    return false;
}
