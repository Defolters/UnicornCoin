#include "keygenerator.h"

#define CRYPTOPP_DEFAULT_NO_DLL
#include "dll.h"
#ifdef CRYPTOPP_WIN32_AVAILABLE
#include <windows.h>
#endif
#include "md5.h"
#include "cryptlib.h"
#include "osrng.h"
#include "eccrypto.h"
#include "oids.h"

#include <QDebug>
#include <string>
#include <QString>
#include <QCryptographicHash>
KeyGenerator::KeyGenerator() {}

QByteArray KeyGenerator::generatePrivateKey()
{
    /* truePrivateKey = sha256(words);
     * checksum = first 4 bytes of sha256(sha256(truePrivateKey))
     * truePrivateAndChecksum = truePrivateKey + checksum
     * privateKey = base58(truePrivateAndChecksum)
    */
    CryptoPP::AutoSeededRandomPool autoSeededRandomPool;
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
    privateKey.Initialize(autoSeededRandomPool, CryptoPP::ASN1::secp160r1());

    bool result = privateKey.Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "private key is not valid!";
        return QByteArray();
    }

    const CryptoPP::Integer &privateKeyInteger = privateKey.GetPrivateExponent();

    QByteArray privateKeyByteArray((int)privateKeyInteger.ByteCount(), 0);
    privateKeyInteger.Encode((byte*)privateKeyByteArray.data(), privateKeyInteger.ByteCount());
    QByteArray m_privateKey = privateKeyByteArray;

    qDebug() << "private key:" << m_privateKey.size() << m_privateKey.toHex();

    // Generating matching public key
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    result = publicKey.Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "public key is not valid!";
        return QByteArray();
    }

    const CryptoPP::ECP::Point& point = publicKey.GetPublicElement();

    QByteArray publicKeyXByteArray((int)point.x.ByteCount(), 0);
    point.x.Encode((byte*)publicKeyXByteArray.data(), point.x.ByteCount());

    QByteArray publicKeyYByteArray((int)point.y.ByteCount(), 0);
    point.y.Encode((byte*)publicKeyYByteArray.data(), point.y.ByteCount());

    QByteArray m_publicKey;
    m_publicKey.clear();
    m_publicKey.append(publicKeyXByteArray);
    m_publicKey.append(publicKeyYByteArray);

    qDebug() << "public key:" << m_publicKey.size() << m_publicKey.toHex();

    //CryptoPP::HexEncoder encoder(new CryptoPP::FileSink( "private.ec.der", false /*binary*/ ));

    //privateKey.Save(encoder);


    //std::string spki;
    //CryptoPP::StringSink ss(spki);

//    CryptoPP::RSA::PublicKey publicKey(...);
    //privateKey.Save(ss);
    //qDebug() << QString::fromStdString(spki);
    //QByteArray a = QByteArray::fromHex(m_privateKey.toHex());
    // randomly generate private key and then..
    return m_privateKey;
    //return QString("5KYZdUEo39z3FPrtuX2QbbwGnNP5zTd7yyr2SC1j299sBCnWjss"); //len = 51
}

QByteArray KeyGenerator::generatePublicKey(QByteArray privateKeyByte)
{
    /*
     * x, y = privateKey * eleptic curve
     * flag = 01 if y is odd and 02 if y is even
     * publicKey = flag + x*/
    qDebug () << "private key for signing:" << privateKeyByte.toHex();
    CryptoPP::AutoSeededRandomPool autoSeededRandomPool;

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
    privateKey.Initialize(autoSeededRandomPool, CryptoPP::ASN1::secp160r1());

    CryptoPP::Integer privateKeyInteger = 0;
    privateKeyInteger.Decode((CryptoPP::byte*)privateKeyByte.data(), (size_t)privateKeyByte .size());
    privateKey.SetPrivateExponent(privateKeyInteger);

    bool result = privateKey.Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "private key is not valid!";
        return QByteArray();
    }


    // Generating matching public key
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    result = publicKey.Validate( autoSeededRandomPool, 3 );
    if( !result )
    {
        qDebug() << "public key is not valid!";
        return QByteArray();
    }

    const CryptoPP::ECP::Point& point = publicKey.GetPublicElement();

    QByteArray publicKeyXByteArray((int)point.x.ByteCount(), 0);
    point.x.Encode((byte*)publicKeyXByteArray.data(), point.x.ByteCount());

    QByteArray publicKeyYByteArray((int)point.y.ByteCount(), 0);
    point.y.Encode((byte*)publicKeyYByteArray.data(), point.y.ByteCount());

    QByteArray m_publicKey;
    m_publicKey.clear();
    m_publicKey.append(publicKeyXByteArray);
    m_publicKey.append(publicKeyYByteArray);

    qDebug() << "public key:" << m_publicKey.size() << m_publicKey.toHex();
    return m_publicKey;
}

QByteArray KeyGenerator::generateAddress(QByteArray publicKey)
{
    /*
     * hashOfPublic = sha256(publicKey)
     * dobleHashOfPublic = ripe160(hashOfPublic)
     * checksum = sha256(sha256(dobleHashOfPublic))
     * doubleHashOfPublicAndChecksum = dobleHashOfPublic + checksum
     * address = base58(doubleHashOfPublicAndChecksum)*/
    // hash public key in order to get short address
    //return QByteArray("1HZwkjkeaoZfTSaJxDw6aKkxp45agDiEzN");
    return QCryptographicHash::hash(publicKey.toHex(), QCryptographicHash::Md5);
}
