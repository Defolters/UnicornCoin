#include "transactionmanager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QCryptographicHash>

#include <cryptlib.h>
#include <osrng.h>
#include <eccrypto.h>
#include <oids.h>


QJsonObject TransactionManager::createNewTransaction(int type,
                                                     QList<QJsonObject> inputs,
                                                     QByteArray recipient,
                                                     QByteArray privateKey,
                                                     QByteArray publicKey,
                                                     QByteArray address,
                                                     double amount, double fee,
                                                     QString message)
{
    QJsonObject tx;
    tx["type"] = type;
    tx["fee"] = fee;
    tx["message"] = message;

    // ¬ј∆Ќјя „ј—“№
    tx["pubkey"] = QString::fromLatin1(publicKey.toBase64());


    QJsonArray in;

    for (int i =0;i<inputs.size();i++)
    {
        QJsonObject input;
        input["value"] = inputs.at(i)["value"];
        // hash and number of output
        input["pubkey"] = QString(publicKey.toHex());

        in << input;
    }
    // in добавить потом, проверить, что хэш до и после добавлени€ одинаковый
    tx["in"] = in;

    QJsonArray out;
    QJsonObject output;
    output["value"] = amount;
    output["recipient"] = QString::fromUtf8(recipient);

    out << output;
    // if sum of inputs > amount + fee
    double sum = 0;

    for(auto i : inputs)
    {
        sum += i["value"].toDouble();
    }

    // остаток вернуть себе
    if (sum > (amount + fee))
    {
        QJsonObject remains;
        remains["value"] = sum - (amount+fee);
        remains["recipient"] = QString::fromUtf8(address);

        out << remains;
    }
    // add in output
    tx["out"] = out;


    QJsonDocument txJD(tx);

    QByteArray signature = signTransaction(txJD.toJson(), privateKey);
    qDebug() << "signa: " << signature;
    // in string and reverse
    QString signastr = QString::fromLatin1(signature.toBase64());
    qDebug() << "signastr: " << signastr;
    QByteArray fromstring = QByteArray::fromBase64(signastr.toLatin1());
    qDebug() << "fromstring: " << fromstring;

    tx["signature"] = QString::fromLatin1(signature.toBase64());


    qDebug() << tx;

    if (verifyTransaction(tx))
    {
        qDebug() << "yee";
    }
    else{qDebug()<<"NOOOO!!";}

    QJsonDocument txret(tx);
    QByteArray hash = QCryptographicHash::hash(txret.toJson(), QCryptographicHash::Sha3_256);
    tx["hash"] = QString::fromLatin1(hash.toBase64());
    return tx;
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

    return m_signature;
}

bool TransactionManager::verifyTransaction(QJsonObject tx)
{
    // chech that public key подходит дл€ tx без signature
    /*QByteArray temp = tx["in"].toArray().at(0).toObject()["pubkey"].toString().toUtf8();
    QByteArray pubKey = QByteArray::fromHex(temp);
    temp = tx["signature"].toString().toUtf8();
    QByteArray signature = QByteArray::fromHex(temp);
    QJsonObject txForVerifying = tx;
    */

    //QJsonDocument txJD(txForVerifying);

    // получаем сигнатуру из транзакции
    QByteArray signature = QByteArray::fromBase64(tx["signature"].toString().toLatin1());
    // получаем публичный ключ
    QByteArray pubKey = QByteArray::fromBase64(tx["pubkey"].toString().toLatin1());
    // копируем транзакцию
    QJsonObject txForVerifying = tx;
    txForVerifying.remove(QString("signature"));
    // переводим в bytearray
    QJsonDocument txJD(txForVerifying);
    QByteArray dataForSign = txJD.toJson();


    CryptoPP::AutoSeededRandomPool autoSeededRandomPool;

    QByteArray xData = pubKey.left(pubKey.size() / 2);
    QByteArray yData = pubKey.right(pubKey.size() / 2);

    qDebug() << xData.toHex() << yData.toHex();

    CryptoPP::ECP::Point point;

    point.x.Decode((CryptoPP::byte*)xData.data(), (size_t)xData.size());
    point.y.Decode((CryptoPP::byte*)yData.data(), (size_t)yData.size());
    point.identity = false;

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;

    publicKey.Initialize( CryptoPP::ASN1::secp160r1(), point );

    bool result = publicKey.Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "public key is not valid!";
        return false;
    }

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::Verifier verifier( publicKey );


//    QByteArray message = txJD.toJson();
    QByteArray message = dataForSign;
    qDebug() << signature;
    result = verifier.VerifyMessage( (const CryptoPP::byte*)message.data(), message.size(), (const CryptoPP::byte*)signature.data(), signature.size() );

    if(result)
    {
        qDebug() << "Verified signature on message";
        return true;
    }
    else
    {
        qDebug() << "Failed to verify signature on message";
        return false;
    }

    return false;
}
