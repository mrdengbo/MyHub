#ifndef QUERYFRIENDDIALOG_H
#define QUERYFRIENDDIALOG_H

#include <QDialog>
#include <QEvent>
#include "maindialog_2.h"

namespace Ui {
class QueryFriendDialog;
}

class QueryFriendDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QueryFriendDialog(MainDialog_2 *main_dialog, QWidget *parent = 0);
    ~QueryFriendDialog();

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    QToolButton *getToolButtonByUserInfo(UserInfo &user_info);
    QToolButton *getToolButton(int num);

private slots:
    void on_toolButton_clicked();

private:
    Ui::QueryFriendDialog *ui;

    MainDialog_2 *m_main_dialog;

    UserList *m_user_list;

    ToolList *m_tool_list;
};

#endif // QUERYFRIENDDIALOG_H
