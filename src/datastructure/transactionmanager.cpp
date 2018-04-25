#include "transactionmanager.h"

#include <QDebug>
#include <QJsonDocument>
#include <QCryptographicHash>

#include <cryptlib.h>
#include <osrng.h>
#include <eccrypto.h>
#include <oids.h>


QJsonObject TransactionManager::createNewTransaction(QList<QJsonObject> inputs,
                                                     QString recipient,
                                                     QByteArray privateKey,
                                                     QByteArray publicKey,
                                                     QString address,
                                                     double amount, double fee,
                                                     QString message)
{
    QJsonObject tx;
    tx["type"] = 1; // tx
    tx["fee"] = fee;
    tx["message"] = message; // no more 39 char

    // ������ ����� (��������� � input ��� ��������)
    tx["pubkey"] = QString(publicKey.toHex());
    qDebug() << "pbkey" << QString(publicKey.toHex());
    qDebug() << "public data key" << publicKey;

    QJsonArray in;

    for (int i =0;i<inputs.size();i++)
    {
        QJsonObject input;
        input["value"] = inputs.at(i)["value"].toInt();
        // ������ ���
        input["pubkey"] = QString(publicKey.toHex());
        // hash and number of output
        input["hash"] = inputs.at(i)["hash"].toString();
        input["index"] = inputs.at(i)["index"].toInt();

        in << input;
    }
    // in �������� �����, ���������, ��� ��� �� � ����� ���������� ����������
    tx["in"] = in;

    QJsonArray out;
    QJsonObject output;
    output["value"] = amount;
    output["recipient"] = recipient;

    out << output;
    // if sum of inputs > amount + fee
    double sum = 0;

    for(auto i : inputs)
    {
        sum += i["value"].toDouble();
    }

    // ������� ������� ����
    QJsonObject remains;
    if (sum > (amount + fee))
    {

        remains["value"] = sum - (amount+fee);
        remains["recipient"] = address;
        qDebug() << "RECIPPIENT" << address;
        out << remains;
    }
    // add in output
    tx["out"] = out;


    QJsonDocument txJD(tx);

    QByteArray signature = signTransaction(txJD.toJson(), privateKey);
    qDebug() << "tx dataForSig: " << txJD.toJson();
    qDebug() << "signa: " << signature;
    // in string and reverse
    QString signastr = QString(signature.toHex());
    qDebug() << "signastr: " << signastr;
    QByteArray fromstring = QByteArray::fromHex(signastr.toUtf8());
    qDebug() << "fromstring: " << fromstring;

    tx["signature"] = QString(signature.toHex());


    qDebug() << tx;

    if (verifyTransaction(tx))
    {
        qDebug() << "tx verified";
    }
    else{qDebug()<<"tx not verified!!";}

    QJsonDocument txret(tx);
    QByteArray hash = QCryptographicHash::hash(txret.toJson(), QCryptographicHash::Sha3_256);
    qDebug() << "HASHOFTX!!!!!!"<<QString(hash.toHex());
    tx["hash"] = QString(hash.toHex());

    return tx;
}

QJsonObject TransactionManager::createCoinbaseTransaction(QString recipient, double amount)
{
    qDebug() << Q_FUNC_INFO;

    // �������� (��������� ������� + fee)
    // � ����� ������ ���� ����� ���������� (������)
    // ������� ������� ��� �������� ���������� (���� � ����)
    // �������� ��������� ����� ��� ���� ����� ����� ������ ()
    QJsonObject tx;

    tx["type"] = 0; // block reward
    tx["message"] = QString("Block reward");

    QJsonArray out;
    QJsonObject output;
    output["value"] = amount;
    output["recipient"] = recipient;
    out << output;

    tx["out"] = out;

    qDebug() << "MINERADDRESS TXMANAGER" << recipient;

    QJsonDocument txJD(tx);

    QByteArray signature = signTransaction(txJD.toJson(), QByteArray("0",1));
    qDebug() << "tx dataForSig: " << txJD.toJson();
    qDebug() << "signa: " << signature;
    // in string and reverse
    QString signastr = QString(signature.toHex());
    qDebug() << "signastr: " << signastr;
    QByteArray fromstring = QByteArray::fromHex(signastr.toUtf8());
    qDebug() << "fromstring: " << fromstring;

    tx["signature"] = QString(signature.toHex());



    QJsonDocument txret(tx);
    QByteArray hash = QCryptographicHash::hash(txret.toJson(), QCryptographicHash::Sha3_256);
    qDebug() << QString(hash.toHex());
    tx["hash"] = QString(hash.toHex());

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
    // ��������� ��������� � ������ ���� (contains)
    // ����������� ������ � ��������� ���������??

    // chech that public key �������� ��� tx ��� signature
    /*QByteArray temp = tx["in"].toArray().at(0).toObject()["pubkey"].toString().toUtf8();
    QByteArray pubKey = QByteArray::fromHex(temp);
    temp = tx["signature"].toString().toUtf8();
    QByteArray signature = QByteArray::fromHex(temp);
    QJsonObject txForVerifying = tx;
    */

    //QJsonDocument txJD(txForVerifying);

    // �������� ��������� �� ����������
    QByteArray signature = QByteArray::fromHex(tx["signature"].toString().toUtf8());
    qDebug() << "verify signa: " << signature;
    // �������� ��������� ����
    QByteArray pubKey = QByteArray::fromHex(tx["pubkey"].toString().toUtf8());
    qDebug() << "verify pub: " << pubKey;
    // �������� ����������
    QJsonObject txForVerifying = tx;
    txForVerifying.remove(QString("signature"));
    txForVerifying.remove(QString("hash"));
    // ��������� � bytearray
    QJsonDocument txJD(txForVerifying);
    QByteArray dataForSign = txJD.toJson();

    qDebug() << "verify dataForSig: " << dataForSign;

    qDebug() << "pbkey" << pubKey;


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
