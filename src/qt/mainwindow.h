#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>

#include "unicorncoin.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:
    void networkPage(int peer);
    void change_data(QString array, QString label);

private slots:
    //! Slot processes new data
    void newData(const MessageType &type, const QString &data);
    //! Slot processes new reqest
    void newRequest(const MessageType &type, const QString &data, Connection *connection);

    void on_pushButton_4_clicked();

    void on_actionwallet_triggered();

    void on_actionminer_triggered();

    void on_actionsend_triggered();

    void on_actionhistory_triggered();

    void on_actiondatabase_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_send_clicked();

    void on_actionnetwork_triggered();

    void on_generateNewAddressRP_clicked();

    void on_createTransaction_clicked();

    void on_addExistingAddressRP_clicked();

private:
    Ui::MainWindow *ui;
    UnicornCoin uniCoin;

};

#endif // MAINWINDOW_H
