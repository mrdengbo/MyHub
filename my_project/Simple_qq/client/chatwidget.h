#ifndef CHATWIDGET_H
#define CHATWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QTextBrowser>
#include <QFontComboBox>
#include <QComboBox> /*选项栏控件*/
#include <QPushButton>
#include <QToolButton>
#include <QLineEdit>
#include <QColor>
#include <QIcon>
#include <QFont>

/*自定义的类使用前置声明*/
class MainDialog_2;

class ChatWidget : public QWidget
{
    Q_OBJECT
public:
    //explicit ChatWidget(QWidget *parent = nullptr);
    explicit ChatWidget(int id, const QIcon &icon, const QString &send_name, const QString &recv_name, int status, MainDialog_2 *main_dialog, QWidget *parent = nullptr);
    //explicit ChatWidget(const ChatWidget &chat_widget);
    ~ChatWidget();
    int chat_id() const { return m_id; }
    void setSendName(const QString &send_name) { m_send_name = send_name; }
    void setRecvName(const QString &recv_name) { m_recv_name = recv_name; }
    void setStatus(int status) { m_status = status; }
    void refreshChatWidget(int status, const QString &send_name, const QString &recv_name);
    void addpendTextBrower(const QString &name, const QString &dialogue);

private:
    /*初始化所有控件*/
    void initAllWidgets();
    /*建立指定控件槽函数*/
    void establishConnect();
    /*编辑数据写入文本浏览器*/
    void writeTextBrower(const QString &name, const QString &msg);
    //void stringToHtmlFilter(QString &str);
    //void stringToHtml(QString &str,QColor crl);
    void imgPathToHtml(QString &path);

private slots:
    void onMainTool();
    void onFontCombobox(const QFont &);
    void onSizeCombobox();
    void onColorTool();
    void onBoldTool(bool);
    void onUnderTool(bool);
    void onItalicTool(bool);
    void onHistoryPush();
    void onLineEdit();
    void onSendPush();

private:
    int m_id;                       /*用户ID标识*/
    int m_status;                   /*设置用户对应登录状态*/
    QString m_send_name;            /*发送方的昵称*/
    QString m_recv_name;            /*接收方的昵称*/
    QByteArray *m_array;            /*字符串缓冲区*/
    MainDialog_2 *m_handle;         /*主界面句柄*/
    QColor *m_color;                /*颜色对象*/

    QToolButton *m_main_tool;       /*返回主界面*/
    QTextBrowser *m_text_brower;    /*消息浏览器*/
    QFontComboBox *m_font_combobox; /*字体选项*/
    QComboBox *m_size_combobox;     /*字号大小*/
    QToolButton *m_color_tool;      /*字体颜色*/
    QToolButton *m_bold_tool;       /*字体加粗*/
    QToolButton *m_underline_tool;  /*字体下划线*/
    QToolButton *m_italic_tool;     /*字体斜体*/
    QPushButton *m_history_push;    /*消息记录*/
    QLineEdit *m_line_edit;         /*消息编辑栏*/
    QPushButton *m_send_push;       /*发送消息*/
};

#endif // CHATWIDGET_H
