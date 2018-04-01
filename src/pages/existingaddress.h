#ifndef EXISTINGADDRESS_H
#define EXISTINGADDRESS_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class Dialog;
}

/**
 * @brief The ExistingAddress class implements GUI of adding existing address
 */
class ExistingAddress : public QDialog
{
    Q_OBJECT

public:
    explicit ExistingAddress(QWidget *parent = nullptr);

signals:
    //!
    //! \brief Signal is emitted when user passed private key
    //! \param privateKey
    //!
    void existingPrivate(QString privateKey);

private slots:
    //!
    //! \brief Slot is called when button on buttonBox is pressed
    //!
    void on_buttonBox_accepted();

private:
    //! User interface
    Ui::Dialog *ui;
};

#endif // EXISTINGADDRESS_H
