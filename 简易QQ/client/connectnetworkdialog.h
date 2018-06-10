#ifndef CONNECTNETWORKDIALOG_H
#define CONNECTNETWORKDIALOG_H

#include <QDialog>
#include <QString>
#include <QCloseEvent>

namespace Ui {
class ConnectNetworkDialog;
}

class ConnectNetworkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectNetworkDialog(QWidget *parent = 0);
    ~ConnectNetworkDialog();

    QString hostip;
    QString port;
    bool isClick;

private slots:
    void on_toolButton_clicked();

private:
    Ui::ConnectNetworkDialog *ui;
};

#endif // CONNECTNETWORKDIALOG_H
