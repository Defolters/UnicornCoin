#include "transaction.h"
#include <QDebug>
#include <fstream>
#include <iostream>
Transaction::Transaction()
{

}

Transaction::Transaction(QString address, double amount, double fee)
{
    exportFile["address"] = address.toUtf8().constData();
    exportFile["amount"] = amount;
    exportFile["fee"] = fee;
    /*json j;
    i >> j;*/

    // write prettified JSON to another file
//    std::ofstream o("pretty.json");
    //o <<   std::setw(4) << exportFile << std::endl;
    qDebug() << QString::fromStdString(exportFile.dump(4));
//    qDebug() << QString(exportFile.dump());
}


Transaction::~Transaction()
{

}

bool Transaction::makeRawData()
{
    return false;
}

bool Transaction::sendToNetwork()
{
    return false;
}
