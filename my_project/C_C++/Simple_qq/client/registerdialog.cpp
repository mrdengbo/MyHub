#include "registerdialog.h"
#include "ui_registerdialog.h"
#include <QMessageBox>

RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("注册系统"));
    setFixedSize(440, 260);
    isClick = false;
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_pushButton_clicked()
{
    /*将编辑栏的内容写入字符串*/
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("账号不能为空"));
        ui->lineEdit->setFocus();
    }
    else if (ui->lineEdit->text().size() > 12)
    {
        QMessageBox::warning(this, tr("错误"), tr("账号内容不能超过12个字符"));
        ui->lineEdit->setFocus();
    }
    else
    {
        bool isDigit;
        ui->lineEdit->text().toInt(&isDigit);
        if (isDigit)
        {
            isClick = true;
            account = ui->lineEdit->text();
            passwd = ui->lineEdit_2->text();
            /*关闭注册窗口*/
            close();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("账号必须为数字"));
            ui->lineEdit->setFocus();
        }
    }
}

void RegisterDialog::on_pushButton_2_clicked()
{
    close();
}
