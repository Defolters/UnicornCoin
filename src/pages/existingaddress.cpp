#include "existingaddress.h"
#include "ui_dialog.h"
#include <QString>
#include <QDebug>

ExistingAddress::ExistingAddress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    setFont(QFont ("Calibri Light", 9));
    ui->lineEdit->setFocus();
}

void ExistingAddress::on_buttonBox_accepted()
{
    QString privateKey = ui->lineEdit->text();

    emit existingPrivate(privateKey);
}
