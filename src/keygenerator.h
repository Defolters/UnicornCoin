#ifndef KEYGENERATOR_H
#define KEYGENERATOR_H
#include <QString>

class KeyGenerator
{
public:
    KeyGenerator();
    void SetPrivateKey();
    QString generatePrivateKey();
    QString generatePrivateKeyFromWords(string words);
    QString generatePublicKey();
    QString generateAddress();

private:
    QString privateKey;
    QString publicKey;
    QString address;
};

#endif // KEYGENERATOR_H
