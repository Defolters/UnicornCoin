#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>

#include "unicorncoin.h"

namespace Ui {
class MainWindow;
}

/**
 * \brief The MainWindow class
 * GUI implementation of program
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:
    //!
    //! \brief networkPage
    //! \param peer
    //!
    void networkPage(int peer);

private slots:
    //!
    //! \brief Slot processes new data
    //! \param type
    //! \param data
    //!
    void newData(const DataType &type, const QString &data);

    //!
    //! \brief Slot processes new reqest
    //! \param type
    //! \param data
    //! \param connection
    //!
    void newRequest(const DataType &type, const QString &data,
                    Connection *connection);

    //!
    //! \brief existingPrivate
    //! \param privateKey
    //!
    void existingPrivate(QString privateKey);

    void on_pushButton_4_clicked();

    void on_actionwallet_triggered();

    void on_actionsend_triggered();

    void on_actionhistory_triggered();

    void on_pushButton_2_clicked();

    void on_pushButton_send_clicked();

    void on_actionnetwork_triggered();

    void on_generateNewAddressRP_clicked();

    void on_createTransaction_clicked();

    void on_addExistingAddressRP_clicked();

    void on_copyClipboard_clicked();

    void newBalance(double balance, double unconfirmed);

protected:
    //!
    //! \brief closeEvent
    //! \param event
    //!
    void closeEvent(QCloseEvent *event) override;

private:
    //!
    //! \brief updateAddress
    //!
    void updateAddress();

    //!
    //! \brief Method saves settings of application
    //!
    void save();

    //!
    //! \brief Method loads settings of application
    //!
    void load();

    //!
    Ui::MainWindow *ui;

    //!
    UnicornCoin uniCoin;

    int wow = 0;
};

#endif // MAINWINDOW_H
