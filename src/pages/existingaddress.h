#ifndef EXISTINGADDRESS_H
#define EXISTINGADDRESS_H

#include <QWidget>
#include <QDialog>

namespace Ui {
class Dialog;
}

class ExistingAddress : public QDialog
{
    Q_OBJECT

public:
    explicit ExistingAddress(QWidget *parent = nullptr);

signals:
    //!
    void existingPrivate(QString privateKey);

private slots:
    //!
    void on_buttonBox_accepted();

private:
    //!
    Ui::Dialog *ui;
};

#endif // EXISTINGADDRESS_H
