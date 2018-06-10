#ifndef MAINDIALOG_2_H
#define MAINDIALOG_2_H

#include <QDialog>
#include <QToolBox>
#include <QWidget>
#include <QPushButton>
#include <QToolButton>
#include <QLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QList>
#include <QDebug>
#include <QPixmap>
#include <QIcon>
#include <QGroupBox>
#include <QMouseEvent>
#include <QEvent>
#include <QByteArray>
#include <QTcpSocket>
#include <QAction>
#include <QLabel>
#include <QTimer>

#include "mymacro.h"
#include "logindialog.h"

typedef QList<UserInfo> UserList;
typedef QList<QToolButton *> ToolList;

class ChatWidget;

namespace Ui {
class MainDialog_2;
}

class MainDialog_2 : public QDialog
{
    Q_OBJECT
public:
    typedef QList<ChatWidget *> ChatList;
    explicit MainDialog_2(QWidget *parent = 0);
    ~MainDialog_2();

    void sendMsgToFriend(int id, QByteArray &dialogue);
    QToolButton *getToolButtonByUserInfo(UserInfo *user_info);

    friend const UserList &getOnlineFriendUser(MainDialog_2 *main_dialog);

protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    void initUiWidget();
    void initSocket();
    void connectNetwork();
    void loginInterface();
    QToolButton *createToolButton(const UserInfo *user_info);
    QToolButton *getToolButton(int num);
    ChatWidget *getChatWidget(int num);
    void addAccountPasswd(const QString &account, const QString &passwd);
    void addUserInfo(const UserInfo *user_info);
    void addDatagramHead(int length);
    void sendAccounPasswd();
    void dealLogin(const MsgControl &msg, const char *str);
    void dealRegister(const MsgControl &msg, const char *str);
    void dealUpdateUserInfo(const MsgControl &msg, const UserInfo *user_info);
    void dealDeleteUserInfo(const MsgControl &msg);
    void dealQueryOnlineFriend(const MsgControl &msg, const UserInfo *user_info);
    void dealRequestAddFriend(const MsgControl &msg, const UserInfo *user_info);
    void refreshUserInfo(const MsgControl &msg, const UserInfo *user_info);
    void refreshFriendInfo(const MsgControl &msg, const UserInfo *user_info);
    void refreshToolButton(QToolButton *tool_button, const UserInfo *user_info);
    void dealSend(const MsgControl &msg, const QByteArray &dialogue);
    void refreshMyself();
    void refreshFriend(const UserInfo *user_info);
    void getFriend();
    void recvMsg(int peer_id, const QByteArray &dialogue);
    void updateUserInfo(const QString &name, const QString &indiv);
    void deleteUserInfo();
    void requestOnlineFriend();
    void addFriendResult(const UserInfo *user_info);
    void isAddFriend(int flg);
    void convertPixmapToBytearray(const QPixmap &pixmap);
    void sendImageFile();

private slots:
    void onSocketError(QAbstractSocket::SocketError socket_error);
    void onReadMsg();
    void onFirstConnected();
    void onReboot();
    void onTimeOut();

    void on_toolButton_2_clicked();

    void on_toolButton_6_clicked();

    void on_toolButton_5_clicked();

    void on_toolButton_4_clicked();

private:
    UserInfo *m_user_info;         /*个人用户信息的缓冲区*/
    MsgControl *m_msg_control;     /*消息控制协议结构*/
    QByteArray *m_byte_array;      /*socket读取消息时的缓冲区*/
    QByteArray *m_data_transfer;   /*大型数据传输时的缓冲区*/
    ToolList *m_tool_list;   /*存储好友控件的容器*/
    ChatList *m_chat_list;   /*存储聊天界面的容器*/
    UserList *m_user_list;   /*所有好友用户信息的缓冲区容器*/
    QTimer *m_timer;

    bool hasConnected;       /*判断socket是否连接连接成功*/

public:
    static void disConnectSocket();
    static void sRegisterNewUser(const QString &account, const QString &passwd);
    static void sRequestAddFriend(int id);

private:
    static QTcpSocket *s_socket; /*TCP socket通信对象*/
    static int s_id;       /*客户端用户的唯一ID*/
    static int s_peer_id;  /*添加好友时所引用的对方ID*/

private:
    Ui::MainDialog_2 *ui;
};

#endif // MAINDIALOG_2_H
