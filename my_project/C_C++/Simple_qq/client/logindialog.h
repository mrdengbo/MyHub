#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QString>
#include <QTcpSocket>


class LoginDialog : public QDialog
{
    Q_OBJECT
public:
    explicit LoginDialog(QWidget *parent = 0);

signals:

public slots:

public:
    const QString &account_string() const { return *account_string_; }
    const QString &passwd_string() const { return *passwd_string_; }
    //const QString &host_ip_string() const { return *host_ip_string_; }
    //const QString &port_string() const { return *port_string_; }
    const QString &new_account() const { return *new_account_; }
    const QString &new_passwd() const { return *new_passwd_; }
    bool isLogin;
    //bool isConnect;

private slots:
    void onConfirmPushButton();
    void onCancelPushButton();
    void onRegisterPushButton();
    void onPasswdPushButton();
    //void onConnectPushButton();

private:
    QLabel *account_laber_;
    QLabel *passwd_label_;
    //QLabel *host_ip_label_;
    //QLabel *port_label_;

    QLineEdit *account_line_edit_;
    QLineEdit *passwd_line_edit_;
    //QLineEdit *host_ip_line_edit_;
    //QLineEdit *port_line_edit_;

    //QPushButton *connect_push_button_;
    QPushButton *register_push_button_;
    QPushButton *passwd_push_button_;
    QPushButton *confirm_push_button_;
    QPushButton *cancel_push_button_;

    QString *account_string_;
    QString *passwd_string_;

    //QString *host_ip_string_;
    //QString *port_string_;

    QString *new_account_;
    QString *new_passwd_;

};

#endif // LOGINDIALOG_H
