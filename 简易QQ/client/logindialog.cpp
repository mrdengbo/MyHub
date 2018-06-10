#include <QGridLayout>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPalette>
#include <QBrush>
#include <QPixmap>

#include "logindialog.h"
#include "registerdialog.h"
//#include "connectnetworkdialog.h"
//#include "toolbuttons.h"
#include "maindialog_2.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent)
{
    //setWindowTitle(tr("登录界面"));
    account_string_ = new QString;
    passwd_string_ = new QString;
    //host_ip_string_ = new QString;
    //port_string_ = new QString;
    new_account_ = new QString;
    new_passwd_ = new QString;
    isLogin = false;

    account_laber_ = new QLabel;
    account_laber_->setText(tr("  账号"));
    account_line_edit_ = new QLineEdit;

    passwd_label_ = new QLabel;
    passwd_label_->setText(tr("  密码"));
    passwd_line_edit_ = new QLineEdit;
    /*设置密码框*/
    passwd_line_edit_->setEchoMode(QLineEdit::Password);

    //host_ip_label_ = new QLabel;
    //host_ip_label_->setText(tr("服务器IP"));
    //host_ip_line_edit_ = new QLineEdit;

    //port_label_ = new QLabel;
    //port_label_->setText(tr("服务器端口"));
    //port_line_edit_ = new QLineEdit;

    //connect_push_button_ = new QPushButton;
    //connect_push_button_->setText(tr("联网"));

    register_push_button_ = new QPushButton;
    register_push_button_->setText(tr("注册"));
    passwd_push_button_ = new QPushButton;
    passwd_push_button_->setText(tr("忘记密码"));
    //passwd_push_button_->setToolTip(tr("拉闸"));
    confirm_push_button_ = new QPushButton;
    confirm_push_button_->setText(tr("登录"));
    cancel_push_button_ = new QPushButton;
    cancel_push_button_->setText(tr("取消"));

    /*设置纵向布局主控件*/
    QGridLayout *login_layout_ = new QGridLayout(this);
    /*添加文字标签控件*/
    login_layout_->addWidget(account_laber_, 0, 0);
    /*添加输入栏编辑控件*/
    login_layout_->addWidget(account_line_edit_, 0, 1);

    login_layout_->addWidget(register_push_button_, 0, 2);

    login_layout_->addWidget(passwd_label_, 1, 0);
    login_layout_->addWidget(passwd_line_edit_, 1, 1);

    login_layout_->addWidget(passwd_push_button_, 1, 2);

    //login_layout_->addWidget(host_ip_label_, 2, 0);
    //login_layout_->addWidget(host_ip_line_edit_, 2, 1);

    //login_layout_->addWidget(port_label_, 3, 0);
    //login_layout_->addWidget(port_line_edit_, 3, 1);

    //QHBoxLayout *button_layout_ = new QHBoxLayout(this);
    //button_layout_->addWidget(confirm_push_button_);
    //button_layout_->addWidget(cancel_push_button_);
    //button_layout_->setStretch();

    //login_layout_->addWidget(connect_push_button_, 4, 0);
    login_layout_->addWidget(confirm_push_button_, 4, 1);
    login_layout_->addWidget(cancel_push_button_, 4, 2);

    /*设置第一列占一半*/
    login_layout_->setColumnStretch(0, 1);
    /*设置第二列占一半*/
    login_layout_->setColumnStretch(1, 1);

    login_layout_->setColumnStretch(2, 1);

    /*设置控件边距*/
    login_layout_->setMargin(15);
    /*设置控件间距*/
    login_layout_->setSpacing(10);
    /*设置布局大小适中*/
    login_layout_->setSizeConstraint(QLayout::SetFixedSize);
    /*设置当前编辑栏具有输入焦点*/
    account_line_edit_->setFocus();
    //account_line_edit_->setText(tr("245600445"));
    //passwd_line_edit_->setText(tr("123456"));
    //host_ip_line_edit_->setText(tr("192.168.1.183"));
    //host_ip_line_edit_->setText(tr("192.168.43.119"));
    //port_line_edit_->setText(tr("8181"));
    /*设置无边框*/
    setWindowFlags(Qt::FramelessWindowHint);
    /*设置任务栏不显示*/
    setWindowFlags(windowFlags() | Qt::Tool);
    /*设置自动填充背景色*/
    setAutoFillBackground(true);
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/image/2929309_173614107001_2.jpg")));
    setPalette(palette);

    connect(confirm_push_button_, SIGNAL(clicked()), this, SLOT(onConfirmPushButton()));
    connect(cancel_push_button_, SIGNAL(clicked()), this, SLOT(onCancelPushButton()));
    connect(register_push_button_, SIGNAL(clicked()), this, SLOT(onRegisterPushButton()));
    connect(passwd_push_button_, SIGNAL(clicked()), this, SLOT(onPasswdPushButton()));
    //connect(connect_push_button_, SIGNAL(clicked()), this, SLOT(onConnectPushButton()));
}

void LoginDialog::onConfirmPushButton()
{
    /*将编辑栏的内容写入字符串*/
    if (account_line_edit_->text().isEmpty())
    {
        QMessageBox::information(this, tr("错误"), tr("账号不能为空"));
        account_line_edit_->setFocus();
    }
    else if (account_line_edit_->text().size() > 12)
    {
        QMessageBox::information(this, tr("错误"), tr("账号内容不能超过12个字符"));
        account_line_edit_->setFocus();
    }
    else
    {
        bool isDigit;
        account_line_edit_->text().toInt(&isDigit);
        if (isDigit)
        {
            isLogin = true;
            *account_string_ = account_line_edit_->text();
            *passwd_string_ = passwd_line_edit_->text();
            /*关闭登录窗口*/
            close();
        }
        else
        {
            QMessageBox::information(this, tr("错误"), tr("账号必须为数字"));
            account_line_edit_->setFocus();
        }
    }
}

void LoginDialog::onCancelPushButton()
{
    MainDialog_2::disConnectSocket();
    exit(EXIT_SUCCESS);
}

void LoginDialog::onRegisterPushButton()
{
    RegisterDialog r;
    r.exec();
    if (r.isClick)
    {
        MainDialog_2::sRegisterNewUser(r.account, r.passwd);
    }
}

void LoginDialog::onPasswdPushButton()
{
    QMessageBox::information(this, tr("提示"), tr("拉闸"));
}
