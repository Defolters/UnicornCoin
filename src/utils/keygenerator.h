#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H

#include <QString>
#include <QCryptographicHash>


/**
 * @brief The KeyGenerator class
 * This class generates private and privates keys. Also it converts public key to address.
 */

class KeyGenerator
{
public:
    KeyGenerator();

    //! Method generates random private key and returns QByteArray
    static QByteArray generatePrivateKey();

    //! Method generates public key from private key
    static QByteArray generatePublicKey(QByteArray privateKey);

    //! Method generates address from public key, which displayed for user
    static QByteArray generateAddress(QByteArray publicKey);

    //! Method generate
    //! @return true if
    //! @return false if
    static bool checkAddress(QByteArray address, QByteArray publicKey);

private:

};

#endif // KEYGENERATOR_H

/*
 * https://royalforkblog.github.io/2014/08/11/graphical-address-generator/#hello
 * privateKey = ''.join(['%x' % random.randrange(16) for x in range(0, 64)]) or hash256(passphrase)
 * secretKey = ecdsa.SigningKey.from_string(privateKey.decode('hex'), curve=ecdsa.SECP256k1)
 * verifyingKey = secretKey.VerifyingKey.toString().encode('hex')
 * publicKey = verifyingKey
 * address = BASE58(RIPEMD160(SHA256(publicKey)))
*/
