#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H
#include <QString>
#include <QCryptographicHash>
class KeyGenerator
{
public:
    KeyGenerator();

    QString generatePrivateKey();
    QString generatePrivateKey(QString words);
    QString generatePublicKey(QString privateKey);
    QString generateAddress(QString publicKey);

private:
    /*QString privateKey;
    QString publicKey;
    QString address;*/
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
