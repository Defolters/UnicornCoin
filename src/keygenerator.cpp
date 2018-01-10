#include "keygenerator.h"

KeyGenerator::KeyGenerator():
    privateKey(), publicKey(), address()
{

}

void KeyGenerator::SetPrivateKey()
{

}

QString KeyGenerator::generatePrivateKey()
{
    return QString("32132FSFDFDF");
}

QString KeyGenerator::generatePrivateKeyFromWords(string words)
{
    return QString("32132FSFDFDF");
}

QString KeyGenerator::generatePublicKey()
{
    return QString("3212sdfa3sf1sdf5s41d6f251sd65f1s6df1sdf3612sd1f");
}

QString KeyGenerator::generateAddress()
{
    return QString("3212sdfa3sf1sdf2sd1f");
}
