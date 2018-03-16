#include "keygenerator.h"

#include <QCryptographicHash>
#include <string>

#define CRYPTOPP_DEFAULT_NO_DLL
#include "dll.h"
#ifdef CRYPTOPP_WIN32_AVAILABLE
#include <windows.h>
#endif
#include "cryptlib.h"
#include "osrng.h"
#include "eccrypto.h"
#include "oids.h"
#include "base32.h"


QByteArray KeyGenerator::generatePrivateKey()
{
    CryptoPP::AutoSeededRandomPool ASRandomPool;
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;

    privateKey.Initialize(ASRandomPool, CryptoPP::ASN1::secp160r1());

    if (!privateKey.Validate(ASRandomPool, 3))
    {
        throw std::runtime_error("Private key is not valid, try again");
    }

    const CryptoPP::Integer &privateKeyInteger = privateKey.GetPrivateExponent();

    QByteArray privateKeyData((int)privateKeyInteger.ByteCount(), 0);
    privateKeyInteger.Encode((byte*)privateKeyData.data(), privateKeyInteger.ByteCount());

    return privateKeyData;
}

QByteArray KeyGenerator::generatePublicKey(QByteArray privateKeyByte)
{
    // Initialize private key
    CryptoPP::AutoSeededRandomPool ASRandomPool;

    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PrivateKey privateKey;
    privateKey.Initialize(ASRandomPool, CryptoPP::ASN1::secp160r1());

    CryptoPP::Integer privateKeyInteger = 0;
    privateKeyInteger.Decode((CryptoPP::byte*)privateKeyByte.data(), (size_t)privateKeyByte.size());
    privateKey.SetPrivateExponent(privateKeyInteger);

    bool result = privateKey.Validate(ASRandomPool, 3);
    if(!result)
    {
        throw std::runtime_error("Private key is not valid, try again");
    }

    // Generate public key from private key
    CryptoPP::ECDSA<CryptoPP::ECP, CryptoPP::SHA256>::PublicKey publicKey;
    privateKey.MakePublicKey(publicKey);

    result = publicKey.Validate(ASRandomPool, 3);
    if(!result)
    {
        throw std::runtime_error("Public key is not valid, try again");
    }

    const CryptoPP::ECP::Point& point = publicKey.GetPublicElement();

    QByteArray publicKeyXByteArray((int)point.x.ByteCount(), 0);
    point.x.Encode((byte*)publicKeyXByteArray.data(), point.x.ByteCount());

    QByteArray publicKeyYByteArray((int)point.y.ByteCount(), 0);
    point.y.Encode((byte*)publicKeyYByteArray.data(), point.y.ByteCount());

    QByteArray publicKeyData;
    publicKeyData.clear();
    publicKeyData.append(publicKeyXByteArray);
    publicKeyData.append(publicKeyYByteArray);

    return publicKeyData;
}

QByteArray KeyGenerator::generateAddress(QByteArray publicKey)
{
    QByteArray address;
    address = QCryptographicHash::hash(publicKey, QCryptographicHash::Sha3_256);
    address = QCryptographicHash::hash(address, QCryptographicHash::Sha1);

    return address;
}

bool KeyGenerator::checkAddress(QByteArray address, QByteArray publicKey)
{
    if (address == generateAddress(publicKey))
        return true;

    return false;
}


QString KeyGenerator::toBase32(QByteArray data)
{
    QByteArray checksum = QCryptographicHash::hash(data, QCryptographicHash::Sha256);
    checksum = QCryptographicHash::hash(checksum, QCryptographicHash::Sha256);

    QByteArray dataWithChecksum = data;
    dataWithChecksum.append(checksum.mid(0,4));

    CryptoPP::Base32Encoder encoder;
    std::string encodedStr;
    encoder.Put((byte*)dataWithChecksum.data(), (size_t)dataWithChecksum.size());
    encoder.MessageEnd();

    CryptoPP::word64 size = encoder.MaxRetrievable();
    if (size)
    {
        encodedStr.resize(size);
        encoder.Get((byte*)&encodedStr[0], encodedStr.size());
    }

    return QString::fromStdString(encodedStr);
}

QByteArray KeyGenerator::fromBase32(QString string)
{
    std::string encodedStr = string.toStdString();
    QByteArray decoded;
    std::string decodedStr;

    CryptoPP::Base32Decoder decoder;
    decoder.Put((byte*)encodedStr.data(), encodedStr.size());
    decoder.MessageEnd();

    CryptoPP::word64 size = decoder.MaxRetrievable();
    if(size && size <= SIZE_MAX)
    {
        decodedStr.resize(size);
        decoder.Get((byte*)&decodedStr[0], decodedStr.size());
    }

    decoded = QByteArray::fromStdString(decodedStr);
    QByteArray hash = decoded.mid(decoded.length()-4, 4);
    decoded =  decoded.mid(0,decoded.length()-4);

    QByteArray checksum = QCryptographicHash::hash(decoded, QCryptographicHash::Sha256);
    checksum = QCryptographicHash::hash(checksum, QCryptographicHash::Sha256);

    if (hash != checksum.mid(0,4))
    {
        throw std::runtime_error("Checksum is invalid, you made a mistake");
    }

    return decoded;
}
