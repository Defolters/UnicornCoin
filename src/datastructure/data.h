#ifndef DATA_H
#define DATA_H


class Data
{
public:
    virtual bool makeRawData() = 0; // should return raw data
    virtual bool sendToNetwork() = 0;
};

#endif // DATA_H
