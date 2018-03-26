#ifndef BASE32_H
#define BASE32_H

#include <QCryptographicHash>
#include <QChar>
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

namespace base32 {

//! Method encodes QByteArray to base32 QString
//! @param data any data
//! @return QString with base32 encoding
static QString toBase32(QByteArray data)
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

//! Method decodes base32 QString to QByteArray
//! @param string QString with base32 encoding
//! @return decoded data from string
static QByteArray fromBase32(QString string)
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
        throw std::runtime_error("Checksum is invalid");
    }

    return decoded;
}

//! Method check if string is 32base
//! @param string any string
//! @return true if base32, false otherwise
static bool isBase32(QString string)
{
    //ABCDEFGHIJKMNPQRSTUVWXYZ23456789
    QString alphabet = "ABCDEFGHIJKMNPQRSTUVWXYZ23456789";
    for (QChar letter : string)
    {
        if (!alphabet.contains(letter.toUpper()))
            return false;
    }
    return true;
}

}


#endif // BASE32_H
