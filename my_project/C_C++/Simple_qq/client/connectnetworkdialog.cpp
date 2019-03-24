#include "connectnetworkdialog.h"
#include "ui_connectnetworkdialog.h"
//#include <QPalette>
#include <QMessageBox>

ConnectNetworkDialog::ConnectNetworkDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectNetworkDialog)
{
    ui->setupUi(this);
    setWindowTitle(tr("联网界面"));
    setFixedSize(440, 260);
    setWindowFlags(Qt::FramelessWindowHint);
    //QPalette palette;
    //palette.setColor(QPalette::Background, QColor(Qt::white));
    //this->setPalette(palette);
    ui->toolButton->setToolTip(tr("赶紧联网呦!"));
    //ui->lineEdit->setText(tr("192.168.1.183"));
    //ui->lineEdit->setText(tr("192.168.43.119"));
    //ui->lineEdit_2->setText(tr("8181"));
    isClick = false;
}

ConnectNetworkDialog::~ConnectNetworkDialog()
{
    delete ui;
}

void ConnectNetworkDialog::on_toolButton_clicked()
{
    /*将编辑栏的内容写入字符串*/
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("IP不能为空"));
        ui->lineEdit->setFocus();
    }
    else if (ui->lineEdit->text().toStdString().find('.') == std::string::npos)
    {
        QMessageBox::warning(this, tr("错误"), tr("IP格式错误"));
        ui->lineEdit->setFocus();
    }
    else
    {
        bool isDigit;
        ui->lineEdit_2->text().toInt(&isDigit);
        if (isDigit)
        {
            isClick = true;
            hostip = ui->lineEdit->text();
            port = ui->lineEdit_2->text();
            /*关闭注册窗口*/
            close();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("端口必须为数字"));
            ui->lineEdit->setFocus();
        }
    }
}
