#-------------------------------------------------
#
# Project created by QtCreator 2018-01-04T09:40:02
#
#-------------------------------------------------

QT       += core gui
QT       += network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = UnicornCoin
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    qt/main.cpp \
    qt/mainwindow.cpp \
    datastructure/transaction.cpp \
    network/server.cpp \
    datastructure/block.cpp \
    datastructure/blockchain.cpp \
    utils/keygenerator.cpp \
    pages/wallet.cpp \
    pages/history.cpp \
    utils/verifier.cpp \
    network/client.cpp \
    network/connection.cpp \
    unicorncoin.cpp \
    datastructure/transactionmanager.cpp

HEADERS += \
    qt\mainwindow.h \
    datastructure/transaction.h \
    network/server.h \
    datastructure/block.h \
    datastructure/blockchain.h \
    utils/keygenerator.h \
    datastructure/data.h \
    pages/wallet.h \
    utils/json.hpp \
    pages/history.h \
    utils/verifier.h \
    network/client.h \
    network/connection.h \
    utils/messagetype.h \
    unicorncoin.h \
    datastructure/transactionmanager.h

FORMS += \
    qt/forms/mainwindow.ui

RESOURCES += \
    qt/res.qrc


INCLUDEPATH += $$PWD/../dependency/cryptopp/include/
DEPENDPATH += $$PWD/../dependency/cryptopp/include/
#C:/Users/Defolter/Downloads/cryptopp610/include/cryptopp
#DEPENDPATH += C:/Users/Defolter/Downloads/cryptopp610/include/cryptopp
#LIBS += -L"C:/Users/Defolter/Downloads/cryptopp610/lib" -lcryptopp
LIBS += -L"$$PWD/../dependency/cryptopp/lib" -lcryptopp

VERSION = 0.0.0.1
QMAKE_TARGET_COMPANY = Defolter
QMAKE_TARGET_PRODUCT = product
QMAKE_TARGET_DESCRIPTION = UnicornCoin
QMAKE_TARGET_COPYRIGHT = MIT
