#include <QDebug>
#include <QMessageBox>
#include <QtEndian>
#include <QHostAddress>
#include <QAbstractSocket>
#include <QHBoxLayout>  /*横布局*/
#include <QApplication>
#include <QStringList>
#include <QDir>
#include <QProcess>
#include <QBuffer>

#include "maindialog_2.h"
#include "ui_maindialog_2.h"
#include "chatwidget.h"
#include "connectnetworkdialog.h"
#include "edutdialog.h"
#include "queryfrienddialog.h"

MainDialog_2::MainDialog_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MainDialog_2)
{
    ui->setupUi(this);
    setWindowTitle(tr(TITLE_TEXT));
    setWindowIcon(QIcon(TOOL_BUTTON_IMG));
    setFixedSize(QSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT));
    initUiWidget();

    m_user_info     = new UserInfo;
    m_msg_control   = new MsgControl;
    m_byte_array    = new QByteArray;
    m_tool_list     = new ToolList;
    m_chat_list     = new ChatList;
    m_user_list     = new UserList;
    m_data_transfer = new QByteArray;
    hasConnected    = false;

    m_tool_list->clear();
    m_chat_list->clear();

    m_timer = new QTimer;
    connect(m_timer, SIGNAL(timeout()), this, SLOT(onTimeOut()));

    initSocket();
    connectNetwork();
    loginInterface();

#if 0
    /*控件居中设置*/
    //ui->verticalLayout_2->setAlignment(Qt::AlignHCenter);
    /*控件间隔设置*/


    //initFriendGroup();
    //initMyselfGroup();
    //m_tool_button = new QToolButton;
    //m_tool_button->setIcon(QIcon(":/image/send.png"));
    //m_tool_button->setIconSize(QSize(16, 16));
    //QHBoxLayout *hbox_layout = new QHBoxLayout(this);
    //hbox_layout->addWidget(m_tool_button);
    //hbox_layout->addStretch();
    //m_action = new QAction;
    //m_action->setText("刷新");
    //this->addAction(m_action);
    //m_tool_box = new QToolBox(this);
    //m_tool_box->setSizeIncrement(this->size());
    //m_tool_box->addItem((QWidget *), tr("我的好友"));
    //m_tool_box->addItem((QWidget *)m_myself_group, tr("个人"));
    //this->setLayout(m_friend_vlayout);
    //this->setLayout(m_myself_vlayout);
#endif

}

MainDialog_2::~MainDialog_2()
{
    delete ui;
    delete m_user_info;
    delete m_msg_control;
    delete m_byte_array;
    delete m_tool_list;
    delete m_chat_list;
    delete m_user_list;
    delete m_data_transfer;
    delete m_timer;
    disConnectSocket();
}

void MainDialog_2::sendMsgToFriend(int id, QByteArray &dialogue)
{
    int length = dialogue.length();
    /*若发送的数据字节数大于一次接收的最大值时,则进行分包发送形式*/
    DEBUG_OUT << "length: " << length;
    if (length > (int)AVAILABLE_SIZE)
    {
        while (length)
        {
            memset(m_msg_control, 0, sizeof(MsgControl));
            m_msg_control->client_request = MSG_SEND;
            m_msg_control->head_1 = m_user_info->id;
            m_msg_control->head_2 = id;
            addDatagramHead(DATAGRAM_MSG + AVAILABLE_SIZE);
            m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
            m_byte_array->append(dialogue.toStdString().c_str(), AVAILABLE_SIZE);
            //m_byte_array->append(dialogue);
            /*检查socket文件描述符是否打开*/
            if (s_socket->isOpen())
            {
                /*检查socket连接状态是否为连接状态*/
                if (s_socket->state() == QAbstractSocket::ConnectedState)
                {
                    /*发送数据*/
                    s_socket->write(*m_byte_array);
                }
            }
            dialogue.remove(0, AVAILABLE_SIZE);
            length -= AVAILABLE_SIZE;
        }
    }
    else
    {
        memset(m_msg_control, 0, sizeof(MsgControl));
        m_msg_control->client_request = MSG_SEND;
        m_msg_control->head_1 = m_user_info->id;
        m_msg_control->head_2 = id;
        addDatagramHead(DATAGRAM_MSG + dialogue.length());
        m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
        //m_byte_array->append(dialogue.toStdString().c_str(), dialogue.length());
        m_byte_array->append(dialogue);
        /*检查socket文件描述符是否打开*/
        if (s_socket->isOpen())
        {
            /*检查socket连接状态是否为连接状态*/
            if (s_socket->state() == QAbstractSocket::ConnectedState)
            {
                /*发送数据*/
                s_socket->write(*m_byte_array);
            }
        }
    }
}


QToolButton *MainDialog_2::getToolButtonByUserInfo(UserInfo *user_info)
{
    QToolButton *tool_button = new QToolButton;
    /*固定tool_button控件大小*/
    tool_button->setFixedSize(QSize(TOOL_BUTTON_WIDTH, TOOL_BUTTON_HEIGHT));
    QString title;
    title = QString(TITLE_DESC).arg(user_info->name).arg(user_info->ap.account).arg(user_info->indiv);
    /*设置文本标题*/
    tool_button->setText(title);
    //QPixmap pixmap(filename);
    title.clear();
    title = QString(IMAGE_NAME_PNG).arg(QString("default"));
    /*设置tool_button图像*/
    tool_button->setIcon(QPixmap(title));
    //tool_button_1->setIconSize(pixmap.size());
    /*设置tool_button图像大小*/
    tool_button->setIconSize(QSize(TOOL_ICON_WIDTH, TOOL_ICON_HEIGHT));
    /*设置tool_button自动浮现风格*/
    tool_button->setAutoRaise(true);
    /*设置tool_button文字在图像旁边*/
    tool_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    /*设置鼠标指向提示词*/
    tool_button->setToolTip(user_info->indiv);
    return tool_button;
}

const UserList &getOnlineFriendUser(MainDialog_2 *main_dialog)
{
    DEBUG_OUT << main_dialog->m_user_list->size();
    return *main_dialog->m_user_list;
#if 0
    UserList::iterator iter = main_dialog->m_user_list->begin();
    for (; iter != main_dialog->m_user_list->end(); iter++)
    {
        v_layout->addWidget(main_dialog->getToolButtonByUserInfo(*iter));
        delete *iter;
    }
    main_dialog->m_user_list->clear();
#endif
}

#if 0
void MainDialog_2::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        //getFriend();
        //requestOnlineFriend();
    }
}
#endif

bool MainDialog_2::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        /*监控鼠标点击toolbutton控件弹出聊天界面*/
        ChatWidget *p_chat = getChatWidget(watched->objectName().toInt());
        p_chat->hide();
        /*弹出聊天界面*/
        p_chat->showNormal();
    }
    return QDialog::eventFilter(watched, event);
}

void MainDialog_2::initUiWidget()
{
    ui->label->setText(tr("..."));
    ui->label_2->setText(tr("..."));
    ui->label_3->setText(tr("..."));
    ui->verticalLayout_2->setMargin(MARGIN_SIZE);
    ui->toolButton->setEnabled(false);
    ui->toolButton_2->setEnabled(false);
    ui->toolButton_4->setEnabled(false);
    ui->toolButton_5->setEnabled(false);
    ui->toolButton_6->setEnabled(false);
}

#if 0
void MainDialog_2::closeEvent(QCloseEvent *event)
{
    QMessageBox::StandardButton event_button = QMessageBox::question(this, tr("退出程序"),
                                                               QString(tr("是否退出?")),
                                                               QMessageBox::Yes | QMessageBox::No,
                                                                   QMessageBox::No);

    if (event_button == QMessageBox::Yes)
    {
        if (s_socket->isOpen())
        {
            s_socket->close();
        }
        /*接受退出信号，程序退出*/
        event->accept();
    }
    else
    {
        /*忽略退出信号，程序继续运行*/
        event->ignore();
    }
}
#endif

void MainDialog_2::initSocket()
{
    s_socket = new QTcpSocket(this);
    /*设置socket出错时触发的槽函数*/
    connect(s_socket, SIGNAL(error(QAbstractSocket::SocketError)), this,
            SLOT(onSocketError(QAbstractSocket::SocketError)));
    /*设置socket读取消息触发的槽函数*/
    connect(s_socket, SIGNAL(readyRead()), this, SLOT(onReadMsg()));
    /*设置socket首次连接成功触发的槽函数*/
    connect(s_socket, SIGNAL(connected()), this, SLOT(onFirstConnected()));
}

void MainDialog_2::connectNetwork()
{
    ConnectNetworkDialog c;
    c.exec();
    if (c.isClick)
    {
        QHostAddress hostAddr(c.hostip);
        /*根据IP和端口连接主机*/
        s_socket->connectToHost(hostAddr, c.port.toInt());
    }
}

void MainDialog_2::loginInterface()
{
    if (s_socket->isOpen())
    {
        LoginDialog login;
        /*模式化显示用户登录框*/
        login.exec();
        if (login.isLogin && hasConnected)
        {
            qDebug() << "account:" << login.account_string();
            qDebug() << "passwd:" << login.passwd_string();
            addAccountPasswd(login.account_string(), login.passwd_string());
            sendAccounPasswd();
        }
        else
        {
            QMessageBox::warning(this, tr("错误"), tr("网络异常!"));
            close();
        }
    }
}

QToolButton *MainDialog_2::createToolButton(const UserInfo *user_info)
{
    QToolButton *tool_button = new QToolButton;
    /*设置控件对象名称*/
    /*[static] QString QString::number(int n, int base = 10)*/
    tool_button->setObjectName(QString::number(user_info->id));
    /*安装事件过滤器*/
    tool_button->installEventFilter(this);
    /*固定tool_button控件大小*/
    tool_button->setFixedSize(QSize(TOOL_BUTTON_WIDTH, TOOL_BUTTON_HEIGHT));
    /*设置文本标题*/
    tool_button->setText(QString(TITLE_DESC).arg(user_info->name).arg(user_info->ap.account).arg(user_info->indiv));
    //QPixmap pixmap(filename);
    /*设置tool_button图像*/
    if (QString(user_info->ap.account) == QString(XIAOMA_ACCOUNT))
    {
        tool_button->setIcon(QPixmap(MY_JPG));
    }
    else
    {
        tool_button->setIcon(QPixmap(TOOL_BUTTON_IMG));
    }

    //tool_button_1->setIconSize(pixmap.size());
    /*设置tool_button图像大小*/
    tool_button->setIconSize(QSize(TOOL_ICON_WIDTH, TOOL_ICON_HEIGHT));
    /*设置tool_button自动浮现风格*/
    tool_button->setAutoRaise(true);
    /*设置tool_button文字在图像旁边*/
    tool_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    if (user_info->status == LEAVE)
    {
        tool_button->setEnabled(false);
        /*设置鼠标指向提示词*/
        tool_button->setToolTip(QString(LEAVE_FRIEND_BUTTON_TIP).arg(user_info->indiv));
    }
    else
    {
        tool_button->setEnabled(true);
        /*设置鼠标指向提示词*/
        tool_button->setToolTip(QString(ONLINE_FRIEND_BUTTON_TIP).arg(user_info->indiv));
    }
    /*在容器中追加tool button*/
    m_tool_list->append(tool_button);
    ChatWidget *tmp_chat = new ChatWidget(user_info->id, tool_button->icon(), m_user_info->name, user_info->name, user_info->status, this);
    m_chat_list->append(tmp_chat);
    return tool_button;
}

QToolButton *MainDialog_2::getToolButton(int num)
{
    bool isExist = false;
    ToolList::iterator iter;
    for (iter = m_tool_list->begin(); iter != m_tool_list->end(); iter ++)
    {
        if ((*iter)->objectName().toInt() == num)
        {
            isExist = true;
            break;
        }
    }
    if (!isExist)
        return nullptr;
    return *iter;
}

/*从容器列表中获取指定聊天界面控件*/
ChatWidget *MainDialog_2::getChatWidget(int num)
{
    bool isExist = false;
    ChatList::iterator iter;
    for (iter = m_chat_list->begin(); iter != m_chat_list->end(); iter ++)
    {
        if ((*iter)->chat_id() == num)
        {
            isExist = true;
            break;
        }
    }
    if (!isExist)
        return nullptr;
    return *iter;
}

void MainDialog_2::addAccountPasswd(const QString &account, const QString &passwd)
{
    memset(m_user_info, 0, sizeof(UserInfo));
    strcpy(m_user_info->ap.account, account.toStdString().c_str());
    strcpy(m_user_info->ap.passwd, passwd.toStdString().c_str());
}

void MainDialog_2::addUserInfo(const UserInfo *user_info)
{
    s_id = user_info->id;
    memset(m_user_info, 0, sizeof(UserInfo));
    memcpy(m_user_info, user_info, DATAGRAM_USER);
}

/*封装数据报的包头*/
void MainDialog_2::addDatagramHead(int length)
{
    int host_length, net_length;
    host_length = length;
    /*本机字节序转换为网络字节数(小端->大端)*/
    net_length = qToBigEndian(host_length);
    /*在数据报首端封装包头*/
    m_byte_array->clear();
    m_byte_array->append((const char *)&net_length, DATAGRAM_HEAD);
}

void MainDialog_2::sendAccounPasswd()
{
    memset(m_msg_control, 0, sizeof(MsgControl));
    m_msg_control->client_request = LOGIN;
    addDatagramHead(DATAGRAM_MSG + DATAGRAM_AP);
    m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
    m_byte_array->append((const char *)&m_user_info->ap, DATAGRAM_AP);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(*m_byte_array);
        }
    }
}

void MainDialog_2::dealLogin(const MsgControl &msg, const char *str)
{
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        refreshMyself();
        m_timer->start(1000);
        break;
    case CLIENT_FAILURE:
        if (QMessageBox::warning(this, tr("登录失败"), tr(str)) == QMessageBox::Ok)
        {
            disConnectSocket();
            close();
        }
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::dealRegister(const MsgControl &msg, const char *str)
{
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        QMessageBox::information(this, tr("恭喜"), tr("注册成功"));
        break;
    case CLIENT_FAILURE:
        QMessageBox::warning(this, tr("注册失败"), tr(str));
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::dealUpdateUserInfo(const MsgControl &msg, const UserInfo *user_info)
{
    ChatWidget *tmp = NULL;
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        //QMessageBox::information(this, tr("恭喜"), tr("修改成功"));
        /*更新成员变量的缓存区数据*/
        strcpy(m_user_info->name, user_info->name);
        strcpy(m_user_info->indiv, user_info->indiv);
        ui->label_2->setText(user_info->indiv);
        //DEBUG_OUT << user_info->indiv;
        ui->label_3->setText(user_info->name);
        //DEBUG_OUT << user_info->name;
        ui->label_3->setStyleSheet(HTML_FONT);
        tmp = getChatWidget(s_id);
        tmp->refreshChatWidget(m_user_info->status, m_user_info->name, m_user_info->name);
        break;
    case CLIENT_FAILURE:
        QMessageBox::warning(this, tr("修改失败"), tr((const char *)user_info));
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::dealDeleteUserInfo(const MsgControl &msg)
{
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        QMessageBox::information(this, tr("删除成功"), m_user_info->name + tr("\n你加油!"));
        disConnectSocket();
        close();
        //onReboot();
        break;
    case CLIENT_FAILURE:
        QMessageBox::information(this, tr("删除失败"), tr("O(∩_∩)O哈哈~"));
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::dealQueryOnlineFriend(const MsgControl &msg, const UserInfo *user_info)
{
    UserInfo tmp;
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        memset(&tmp, 0, DATAGRAM_USER);
        memcpy(&tmp, user_info, DATAGRAM_USER);
        m_user_list->append(tmp);
        DEBUG_OUT;
        break;
    case CLIENT_FAILURE:
        QMessageBox::warning(this, tr("搜索好友失败"), tr((const char *)user_info));
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::dealRequestAddFriend(const MsgControl &msg, const UserInfo *user_info)
{
    switch (msg.client_request)
    {
    case REQUEST:
        addFriendResult(user_info);
        break;
    case AGREEMENT:
        QMessageBox::information(this, tr("恭喜"), QString(AGREEMENT_FRIEND).arg(user_info->name));
        break;
    case REFUSE:
        QMessageBox::information(this, tr("抱歉"), QString(REFUSE_FRIEND).arg(user_info->name));
        break;
    case CLIENT_FAILURE:
        QMessageBox::warning(this, tr("添加失败"), tr((const char *)user_info));
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::refreshUserInfo(const MsgControl &msg, const UserInfo *user_info)
{
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        addUserInfo(user_info);
        break;
    case CLIENT_FAILURE:
        //TODO
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::refreshFriendInfo(const MsgControl &msg, const UserInfo *user_info)
{
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        refreshFriend(user_info);
        break;
    case CLIENT_FAILURE:
        //TODO
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::refreshToolButton(QToolButton *tool_button, const UserInfo *user_info)
{
    QString title;
    title = QString(TITLE_DESC).arg(user_info->name).arg(user_info->ap.account).arg(user_info->indiv);
    /*设置文本标题*/
    tool_button->setText(title);
    if (user_info->status == LEAVE)
    {
        tool_button->setEnabled(false);
        /*设置鼠标指向提示词*/
        tool_button->setToolTip(QString(LEAVE_FRIEND_BUTTON_TIP).arg(user_info->indiv));
    }
    else
    {
        tool_button->setEnabled(true);
        /*设置鼠标指向提示词*/
        tool_button->setToolTip(QString(ONLINE_FRIEND_BUTTON_TIP).arg(user_info->indiv));
    }
}

void MainDialog_2::dealSend(const MsgControl &msg, const QByteArray &dialogue)
{
    switch (msg.client_request)
    {
    case CLIENT_SUCCESS:
        recvMsg(msg.head_2, dialogue);
        break;
    case CLIENT_FAILURE:
        //TODO
        break;
    case CLIENT_RECV_TIMEOUT:
        //TODO
        break;
    case CLIENT_SEND_TIMEOUT:
        //TODO
        break;
    default:
        break;
    }
}

void MainDialog_2::refreshMyself()
{
    if (getChatWidget(m_user_info->id) == nullptr)
    {
        ui->toolButton->setObjectName(QString::number(m_user_info->id));
        /*安装事件过滤器*/
        ui->toolButton->installEventFilter(this);
        ui->label->setText(m_user_info->ap.account);
        ui->label_2->setText(m_user_info->indiv);
        ui->label_3->setText(m_user_info->name);
        ui->label_3->setStyleSheet(HTML_FONT);
        /*设置tool_button图像*/
        if (QString(m_user_info->ap.account) == QString(XIAOMA_ACCOUNT))
        {
            ui->toolButton->setIcon(QPixmap(MY_JPG));
        }
        else
        {
            ui->toolButton->setIcon(QPixmap(TOOL_BUTTON_IMG));
        }
        /*设置tool_button图像大小*/
        ui->toolButton->setIconSize(QSize(MY_JPG_WIDTH, MY_JPG_HEIGHT));
        /*设置tool_button自动浮现风格*/
        ui->toolButton->setAutoRaise(true);
        ui->toolButton->setToolTip("自嗨");
        m_tool_list->append(ui->toolButton);
        ChatWidget *tmp_chat = new ChatWidget(m_user_info->id, ui->toolButton->icon(), m_user_info->name, m_user_info->name, m_user_info->status, this);
        m_chat_list->append(tmp_chat);
        if (strncmp(m_user_info->regis, m_user_info->last, 32) != 0)
        {
            QMessageBox::information(this, tr("嗨!"), tr(LOGIN_TIP).arg(m_user_info->name).arg(m_user_info->last));
        }
    }
    else
    {
        ui->label->setText(m_user_info->ap.account);
        ui->label_2->setText(m_user_info->indiv);
        ui->label_3->setText(m_user_info->name);
        ui->label_3->setStyleSheet(HTML_FONT);
    }
}

/*刷新好友列表*/
void MainDialog_2::refreshFriend(const UserInfo *user_info)
{
    ChatWidget *tmp = getChatWidget(user_info->id);
    /*若不存在则添加*/
    if (tmp == nullptr)
    {
        ui->verticalLayout_2->addWidget(createToolButton(user_info));
    }
    /*存在则刷新*/
    else
    {
        refreshToolButton(getToolButton(user_info->id), user_info);
        /*更新聊天界面的登录状态*/
        tmp->refreshChatWidget(user_info->status, m_user_info->name, user_info->name);
    }
}

void MainDialog_2::getFriend()
{
    memset(m_msg_control, 0, sizeof(MsgControl));
    m_msg_control->client_request = REFRESH_FRIEND_INFO;
    addDatagramHead(DATAGRAM_MSG);
    m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(*m_byte_array);
        }
    }
}

void MainDialog_2::recvMsg(int peer_id, const QByteArray &dialogue)
{
    ChatWidget *p_chat = getChatWidget(peer_id);
    //p_chat->hide();
    /*显示当前窗口*/
    p_chat->showNormal();
    p_chat->addpendTextBrower(p_chat->windowTitle(), dialogue);
}

void MainDialog_2::updateUserInfo(const QString &name, const QString &indiv)
{
    UserInfo user;
    memset(&user, 0, DATAGRAM_USER);
    strcpy(user.name, name.toUtf8().data());
    strcpy(user.indiv, indiv.toUtf8().data());
    memset(m_msg_control, 0, sizeof(MsgControl));
    m_msg_control->client_request = UPDATE_USER_INFO;
    addDatagramHead(DATAGRAM_MSG + DATAGRAM_USER);
    m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
    m_byte_array->append((const char *)&user, DATAGRAM_USER);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(*m_byte_array);
        }
    }
}

void MainDialog_2::deleteUserInfo()
{
    memset(m_msg_control, 0, DATAGRAM_MSG);
    m_msg_control->client_request = DELETE_USER_INFO;
    addDatagramHead(DATAGRAM_MSG);
    m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(*m_byte_array);
        }
    }
}

void MainDialog_2::requestOnlineFriend()
{
    memset(m_msg_control, 0, DATAGRAM_MSG);
    m_msg_control->client_request = QUERY_ONLINE_FRIEND;
    addDatagramHead(DATAGRAM_MSG);
    m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(*m_byte_array);
        }
    }
}

void MainDialog_2::addFriendResult(const UserInfo *user_info)
{
    QMessageBox::StandardButton button;
     button = QMessageBox::question(this, tr("通知"),QString(REQUEST_FRIEND).arg(user_info->name).arg(user_info->ap.account),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (button == QMessageBox::Yes)
    {
        /*同意添加*/
        isAddFriend(AGREEMENT);
    }
    else
    {
        /*拒绝添加*/
        isAddFriend(REFUSE);
    }
}

void MainDialog_2::isAddFriend(int flg)
{
    memset(m_msg_control, 0, DATAGRAM_MSG);
    m_msg_control->client_request = REQUEST_ADD_FRIEND;
    m_msg_control->server_reply = flg;
    m_msg_control->head_1 = s_peer_id;
    m_msg_control->head_2 = s_id;
    addDatagramHead(DATAGRAM_MSG);
    m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(*m_byte_array);
        }
    }
}

void MainDialog_2::convertPixmapToBytearray(const QPixmap &pixmap)
{
    QBuffer buffer(m_data_transfer);
    buffer.open(QIODevice::WriteOnly);
    pixmap.save(&buffer, "JPG",1); // writes pixmap into bytes in PNG format
}

#if 1
void MainDialog_2::sendImageFile()
{
    int length = m_data_transfer->length();
    /*若发送的数据字节数大于一次接收的最大值时,则进行分包发送形式*/
    DEBUG_OUT << "length: " << length;
    if (length > (int)AVAILABLE_SIZE)
    {
        while (length)
        {
            memset(m_msg_control, 0, sizeof(MsgControl));
            m_msg_control->client_request = UPLOAD_PICTURE_FILE;
            addDatagramHead(DATAGRAM_MSG + AVAILABLE_SIZE);
            m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
            m_byte_array->append(m_data_transfer->toStdString().c_str(), AVAILABLE_SIZE);
            /*检查socket文件描述符是否打开*/
            if (s_socket->isOpen())
            {
                /*检查socket连接状态是否为连接状态*/
                if (s_socket->state() == QAbstractSocket::ConnectedState)
                {
                    /*发送数据*/
                    s_socket->write(*m_byte_array);
                }
            }
            m_data_transfer->remove(0, AVAILABLE_SIZE);
            length -= AVAILABLE_SIZE;
        }
    }
    else
    {
        memset(m_msg_control, 0, sizeof(MsgControl));
        m_msg_control->client_request = UPLOAD_PICTURE_FILE;
        addDatagramHead(DATAGRAM_MSG + m_data_transfer->length());
        m_byte_array->append((const char *)m_msg_control, DATAGRAM_MSG);
        m_byte_array->append(m_data_transfer->toStdString().c_str(), m_data_transfer->length());
        //m_byte_array->append(m_data_transfer);
        /*检查socket文件描述符是否打开*/
        if (s_socket->isOpen())
        {
            /*检查socket连接状态是否为连接状态*/
            if (s_socket->state() == QAbstractSocket::ConnectedState)
            {
                /*发送数据*/
                s_socket->write(*m_byte_array);
            }
        }
    }
}
#endif

void MainDialog_2::onSocketError(QAbstractSocket::SocketError socket_error)
{
    switch (socket_error)
    {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    default:
        if (QMessageBox::warning(this, tr("网络错误"), s_socket->errorString()) == QMessageBox::Ok)
        {
            disConnectSocket();
            close();
        }
    }
}

void MainDialog_2::onReadMsg()
{
    int bytes, net_length, host_length;
    static MsgControl msg;
    m_byte_array->clear();
    if ((bytes = s_socket->bytesAvailable()) > 0)
    {
        ui->label_4->setText(QString(PER_SECOND).arg(QString::number(bytes)));
        /*循环读取数据报*/
        while (bytes)
        {
            /*读取数据报的包头*/
            if (s_socket->read((char *)&net_length, DATAGRAM_HEAD) == DATAGRAM_HEAD)
            {
                /*对于小端模式的计算机进行转换，对于大端模式的计算机只进行读取*/
                /*网络字节序转换为本机字节序*/
                host_length = qFromBigEndian(net_length);
                bytes -= host_length + DATAGRAM_HEAD;
                /*根据解析数据报的包头字节数长度来读取指定数据字节数*/
                *m_byte_array = s_socket->read(host_length);
                DEBUG_OUT << host_length;
                if (m_byte_array->count() == host_length)
                {
                    memset(&msg, 0, DATAGRAM_MSG);
                    memcpy(&msg, (MsgControl *)m_byte_array->data(), DATAGRAM_MSG);
                    /*数据报中的msg结构体变量移除*/
                    m_byte_array->remove(0, DATAGRAM_MSG);
                    switch (msg.server_reply)
                    {
                    case LOGIN:
                        DEBUG_OUT;
                        dealLogin(msg, m_byte_array->data());
                        break;
                    case REGISTER:
                        DEBUG_OUT;
                        dealRegister(msg, m_byte_array->data());
                        break;
                    case LOGOUT:
                        DEBUG_OUT;
                        break;
                    case REFRESH_USER_INFO:
                        DEBUG_OUT;
                        refreshUserInfo(msg, (UserInfo *)m_byte_array->data());
                        break;
                    case REFRESH_FRIEND_INFO:
                        DEBUG_OUT;
                        refreshFriendInfo(msg, (UserInfo *)m_byte_array->data());
                        break;
                    case MSG_SEND:
                        DEBUG_OUT;
                        dealSend(msg, *m_byte_array);
                        break;
                    case UPDATE_USER_INFO:
                        DEBUG_OUT;
                        dealUpdateUserInfo(msg, (UserInfo *)m_byte_array->data());
                        break;
                    case DELETE_USER_INFO:
                        DEBUG_OUT;
                        dealDeleteUserInfo(msg);
                        break;
                    case QUERY_ONLINE_FRIEND:
                        DEBUG_OUT;
                        dealQueryOnlineFriend(msg, (UserInfo *)m_byte_array->data());
                        break;
                    case REQUEST_ADD_FRIEND:
                        DEBUG_OUT;
                        dealRequestAddFriend(msg, (UserInfo *)m_byte_array->data());
                        break;
                    default:
                        DEBUG_OUT;
                        break;
                    }
                }
            }
        }
    } /*if ((bytes = s_socket->bytesAvailable()) > 0)*/
}

/*首次连接成功槽函数*/
void MainDialog_2::onFirstConnected()
{
    //DEBUG_OUT;
    /*客户端连接成功后触发该槽函数*/
    //sendAccounPasswd();
    //QMessageBox::information(this, tr("恭喜"), tr("网络连接成功!"));
    hasConnected = true;
    ui->toolButton->setEnabled(true);
    ui->toolButton_2->setEnabled(true);
    ui->toolButton_4->setEnabled(true);
    ui->toolButton_5->setEnabled(true);
    ui->toolButton_6->setEnabled(true);
}

/*重启程序槽函数*/
void MainDialog_2::onReboot()
{
    QString program = QApplication::applicationFilePath();
    QStringList arguments = QApplication::arguments();
    QString workingDirectory = QDir::currentPath();
    QProcess::startDetached(program, arguments, workingDirectory);
    QApplication::exit();
}

/*定时器槽函数*/
void MainDialog_2::onTimeOut()
{
    getFriend();
}

/*注册新用户*/
void MainDialog_2::sRegisterNewUser(const QString &account, const QString &passwd)
{
    MsgControl msg;
    AccountPasswd ap;
    QByteArray byte;
    memset(&msg, 0, DATAGRAM_MSG);
    memset(&ap, 0, DATAGRAM_AP);
    msg.client_request = REGISTER;
    strcpy(ap.account, account.toStdString().c_str());
    strcpy(ap.passwd, passwd.toStdString().c_str());
    int host_length, net_length;
    host_length = DATAGRAM_MSG + DATAGRAM_AP;
    /*本机字节序转换为网络字节数(小端->大端)*/
    net_length = qToBigEndian(host_length);
    /*在数据报首端封装包头*/
    byte.append((const char *)&net_length, DATAGRAM_HEAD);
    byte.append((const char *)&msg, DATAGRAM_MSG);
    byte.append((const char *)&ap, DATAGRAM_AP);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(byte);
        }
    }
}

/*全局静态函数,在查询好友控件中调用,用于请求添加好友操作*/
void MainDialog_2::sRequestAddFriend(int id)
{
    MsgControl msg;
    QByteArray byte;
    s_peer_id = id;
    memset(&msg, 0, DATAGRAM_MSG);
    msg.server_reply   = REQUEST;
    msg.client_request = REQUEST_ADD_FRIEND;
    /*添加对方好友的ID*/
    msg.head_1 = s_peer_id;
    msg.head_2 = s_id;
    int host_length, net_length;
    host_length = DATAGRAM_MSG;
    /*本机字节序转换为网络字节数(小端->大端)*/
    net_length = qToBigEndian(host_length);
    /*在数据报首端封装包头*/
    byte.append((const char *)&net_length, DATAGRAM_HEAD);
    byte.append((const char *)&msg, DATAGRAM_MSG);
    /*检查socket文件描述符是否打开*/
    if (s_socket->isOpen())
    {
        /*检查socket连接状态是否为连接状态*/
        if (s_socket->state() == QAbstractSocket::ConnectedState)
        {
            /*发送数据*/
            s_socket->write(byte);
        }
    }
}

/*处理编辑个人信息*/
void MainDialog_2::on_toolButton_2_clicked()
{
    EdutDialog e(QPixmap(ui->toolButton->icon().pixmap(MY_JPG_WIDTH, MY_JPG_HEIGHT)), m_user_info->name, m_user_info->indiv);
    e.exec();
    if (e.isClick)
    {
        /*更新图片上传服务器流程*/
        if (e.hasLoad)
        {
            //TODO
            QMessageBox::information(this, tr("(○o○)"), tr(LOAD_IMG_TIP));
            ui->toolButton->setIcon(e.m_pixmap);
            ui->toolButton->setIconSize(QSize(MY_JPG_WIDTH, MY_JPG_HEIGHT));
        }
        /*只更新昵称和个性签名*/
        else
        {
            updateUserInfo(e.m_name, e.m_indiv);
        }
    }
}

/*删除用户操作*/
void MainDialog_2::on_toolButton_6_clicked()
{
    QMessageBox::StandardButton button = QMessageBox::question(this, tr("慎重"),
                                                               QString(tr(DELETE_TIP)),
                                                               QMessageBox::Yes | QMessageBox::No,
                                                                   QMessageBox::No);

    /*确认删除*/
    if (button == QMessageBox::Yes)
    {
        deleteUserInfo();
    }
    else
    {
        QMessageBox::information(this, tr("(ﾉﾟ▽ﾟ)ﾉ"), m_user_info->name + tr("\n你真棒!"));
    }
}

/*查询在线好友*/
void MainDialog_2::on_toolButton_5_clicked()
{
    m_user_list->clear();
    requestOnlineFriend();
    QueryFriendDialog d(this);
    d.exec();
}

/*重启应用程序*/
void MainDialog_2::on_toolButton_4_clicked()
{
    disConnectSocket();
    onReboot();
}

/*断开连接*/
void MainDialog_2::disConnectSocket()
{
    if (s_socket->isOpen())
    {
        s_socket->close();
    }
}
