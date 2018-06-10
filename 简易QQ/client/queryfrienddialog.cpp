#include "queryfrienddialog.h"
#include "ui_queryfrienddialog.h"
#include <QMessageBox>

QueryFriendDialog::QueryFriendDialog(MainDialog_2 *main_dialog, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QueryFriendDialog),
    m_main_dialog(main_dialog)
{
    ui->setupUi(this);
    m_user_list = new UserList;
    m_tool_list = new ToolList;
    this->setToolTip(CLICK_ADD_FRIEND);
}

QueryFriendDialog::~QueryFriendDialog()
{
    delete ui;
    delete m_user_list;
    delete m_tool_list;
}

bool QueryFriendDialog::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {
        /*监控鼠标点击toolbutton控件*/
        QToolButton *tool = getToolButton(watched->objectName().toInt());
        tool->setEnabled(false);
        MainDialog_2::sRequestAddFriend(watched->objectName().toInt());
        QMessageBox::information(this, tr("提示"), tr("成功发送添加好友请求!"));
    }
    return QDialog::eventFilter(watched, event);
}

void QueryFriendDialog::on_toolButton_clicked()
{
    m_tool_list->clear();
    *m_user_list = getOnlineFriendUser(m_main_dialog);
    if (m_user_list->isEmpty())
    {
        QMessageBox::information(this, tr("提示"), tr("当前没有在线好友!"));
        return ;
    }
    UserList::iterator iter = m_user_list->begin();
    for (; iter != m_user_list->end(); iter++)
    {
        ui->verticalLayout->addWidget(getToolButtonByUserInfo(*iter));
    }
    m_user_list->clear();
}

QToolButton *QueryFriendDialog::getToolButtonByUserInfo(UserInfo &user_info)
{
    QToolButton *tool_button = new QToolButton;
    tool_button->setObjectName(QString::number(user_info.id));
    tool_button->installEventFilter(this);
    /*固定tool_button控件大小*/
    tool_button->setFixedSize(QSize(TOOL_BUTTON_WIDTH, TOOL_BUTTON_HEIGHT));
    QString title;
    title = QString(TITLE_DESC).arg(user_info.name).arg(user_info.ap.account).arg(user_info.indiv);
    /*设置文本标题*/
    tool_button->setText(title);
    //QPixmap pixmap(filename);
    /*设置tool_button图像*/
    tool_button->setIcon(QPixmap(TOOL_BUTTON_IMG));
    /*设置tool_button图像大小*/
    tool_button->setIconSize(QSize(TOOL_ICON_WIDTH, TOOL_ICON_HEIGHT));
    /*设置tool_button自动浮现风格*/
    tool_button->setAutoRaise(true);
    /*设置tool_button文字在图像旁边*/
    tool_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    /*设置鼠标指向提示词*/
    tool_button->setToolTip(QUERY_DIALOG_BUTTON_TIP);
    m_tool_list->append(tool_button);
    return tool_button;
}

QToolButton *QueryFriendDialog::getToolButton(int num)
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
