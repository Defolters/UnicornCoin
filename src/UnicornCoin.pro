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
    network/server.cpp \
    datastructure/blockchain.cpp \
    utils/keygenerator.cpp \
    pages/wallet.cpp \
    pages/history.cpp \
    network/client.cpp \
    network/connection.cpp \
    unicorncoin.cpp \
    datastructure/transactionmanager.cpp \
    datastructure/blockmanager.cpp \
    utils/miner.cpp

HEADERS += \
    qt/mainwindow.h \
    network/server.h \
    datastructure/blockchain.h \
    utils/keygenerator.h \
    pages/wallet.h \
    pages/history.h \
    network/client.h \
    network/connection.h \
    utils/messagetype.h \
    unicorncoin.h \
    datastructure/transactionmanager.h \
    datastructure/blockmanager.h \
    utils/miner.h

FORMS += \
    qt/forms/mainwindow.ui

RESOURCES += \
    qt/res.qrc


INCLUDEPATH += $$PWD/../dependency/cryptopp/include/
DEPENDPATH += $$PWD/../dependency/cryptopp/include/
LIBS += -L"$$PWD/../dependency/cryptopp/lib" -lcryptopp

VERSION = 0.0.0.2
QMAKE_TARGET_COMPANY = Defolter
QMAKE_TARGET_PRODUCT = product
QMAKE_TARGET_DESCRIPTION = UnicornCoin
QMAKE_TARGET_COPYRIGHT = MIT
