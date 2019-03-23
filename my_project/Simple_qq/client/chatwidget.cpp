#include <QDebug>
#include <QLayout>     /*主布局*/
#include <QVBoxLayout> /*竖布局*/
#include <QHBoxLayout> /*横布局*/
#include <QGridLayout> /*栅格布局*/
#include <QPalette>
#include <QColorDialog>
#include <QScrollBar>
#include <QDateTime>

#include "chatwidget.h"
//#include "MainDialog.h"
#include "maindialog_2.h"
#include "mymacro.h"

ChatWidget::ChatWidget(int id, const QIcon &icon, const QString &send_name, const QString &recv_name, int status,
                       MainDialog_2 *main_dialog, QWidget *parent)
    : QWidget(parent),
      m_id(id),
      m_send_name(send_name),
      m_recv_name(recv_name),
      m_status(status),
      m_handle(main_dialog)
{
    this->setWindowTitle(recv_name);
    this->setWindowIcon(icon);
    this->resize(CHAT_WIDTH, CHAT_HEIGHT);
    //this->setWindowIconText(name);
    /*设置最大化最小化按钮无效*/
    this->setWindowFlags(windowFlags()& ~Qt::WindowMaximizeButtonHint & ~Qt::WindowMinimizeButtonHint);
    //填充背景颜色
    this->setAutoFillBackground(true);
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(Qt::darkCyan));
    this->setPalette(palette);
    m_array = new QByteArray;
    initAllWidgets();
    establishConnect();
}

ChatWidget::~ChatWidget()
{
    delete m_array;
}

void ChatWidget::refreshChatWidget(int status, const QString &send_name, const QString &recv_name)
{
    m_status = status;
    m_send_name = send_name;
    m_recv_name = recv_name;
    setWindowTitle(recv_name);
}

void ChatWidget::addpendTextBrower(const QString &name, const QString &dialogue)
{
    writeTextBrower(name, dialogue);
}

void ChatWidget::initAllWidgets()
{
    m_color = new QColor;
    m_main_tool = new QToolButton;
    m_main_tool->setIcon(QIcon(QString(MAIN_TOOL_IMG)));
    m_main_tool->setIconSize(QSize(30, 20));
    m_main_tool->setShortcut(tr("Esc"));
    m_main_tool->setToolTip(tr("显示主界面(Esc)"));
    QHBoxLayout *hbox_layout_1 = new QHBoxLayout;
    hbox_layout_1->addWidget(m_main_tool);
    hbox_layout_1->addStretch();

    m_text_brower = new QTextBrowser;
    /*载入背景图像*/
    m_text_brower->setStyleSheet(TEXT_BROWSER_STYLE_SHEET);
    QHBoxLayout *hbox_layout_2 = new QHBoxLayout;
    hbox_layout_2->addWidget(m_text_brower);

    m_font_combobox = new QFontComboBox;
    m_font_combobox->setEditable(false);
    m_font_combobox->setToolTip(tr("字体"));

    m_size_combobox = new QComboBox;
    m_size_combobox->setToolTip(tr("字号"));
    /*设置可编辑*/
    //m_size_combobox->setEditable(true);
    m_size_combobox->addItem(tr("8.5"));
    m_size_combobox->addItem(tr("10"));
    m_size_combobox->addItem(tr("12.5"));
    m_size_combobox->addItem(tr("13"));
    m_size_combobox->addItem(tr("15"));
    m_size_combobox->addItem(tr("18.5"));
    m_size_combobox->addItem(tr("20"));
    m_size_combobox->addItem(tr("24"));
    m_size_combobox->addItem(tr("26"));
    /*设置当前下标为0*/
    m_size_combobox->setCurrentIndex(2);

    m_color_tool = new QToolButton;
    m_color_tool->setIcon(QIcon(QString(":/image/font_size_16px_1175787_easyicon.net.ico")));
    m_color_tool->setIconSize(QSize(12, 16));
    m_color_tool->setToolTip(tr("颜色"));

    m_bold_tool = new QToolButton;
    m_bold_tool->setIcon(QIcon(QString(":/image/font_16px_1148178_easyicon.net.ico")));
    m_bold_tool->setIconSize(QSize(12, 16));
    m_bold_tool->setCheckable(true);
    m_bold_tool->setToolTip(tr("加粗"));

    m_underline_tool = new QToolButton;
    m_underline_tool->setIcon(QIcon(QString(":/image/font_16px_1148179_easyicon.net.ico")));
    m_underline_tool->setIconSize(QSize(12, 16));
    m_underline_tool->setCheckable(true);
    m_underline_tool->setToolTip(tr("下划线"));

    m_italic_tool = new QToolButton;
    m_italic_tool->setIcon(QIcon(QString(":/image/font_16px_1067845_easyicon.net.ico")));
    m_italic_tool->setIconSize(QSize(12, 16));
    m_italic_tool->setCheckable(true);
    m_italic_tool->setToolTip(tr("斜体"));

    m_history_push = new QPushButton;
    m_history_push->setText(tr("消息记录"));
    m_history_push->setToolTip(HISTORY_TIP);
    QHBoxLayout *hbox_layout_3 = new QHBoxLayout;
    hbox_layout_3->addWidget(m_font_combobox);
    hbox_layout_3->addWidget(m_size_combobox);
    hbox_layout_3->addWidget(m_color_tool);
    hbox_layout_3->addWidget(m_bold_tool);
    hbox_layout_3->addWidget(m_underline_tool);
    hbox_layout_3->addWidget(m_italic_tool);
    hbox_layout_3->addStretch(3);
    hbox_layout_3->addWidget(m_history_push);

    m_line_edit = new QLineEdit;
    m_send_push = new QPushButton;
    m_send_push->setText(tr("发送"));
    /*定义栅格布局*/
    QGridLayout *grid_layout = new QGridLayout;
    grid_layout->addWidget(m_line_edit, 0, 0);
    grid_layout->addWidget(m_send_push, 0, 1);
    /*设置第一列与第二列的大小比*/
    grid_layout->setColumnStretch(0, 5);
    grid_layout->setColumnStretch(1, 1);

    QVBoxLayout *vbox_layout = new QVBoxLayout;
    vbox_layout->addLayout(hbox_layout_1);
    vbox_layout->addLayout(hbox_layout_2);
    vbox_layout->addLayout(hbox_layout_3);
    vbox_layout->addLayout(grid_layout);
    this->setLayout(vbox_layout);

    /*根据字体控件设置文本浏览器的默认字体*/
    m_text_brower->setCurrentFont(m_font_combobox->font());
    /*根据字号控件设置文本浏览器的默认字号大小*/
    m_text_brower->setFontPointSize(m_size_combobox->currentText().toDouble());
    /*设置光标焦点*/
    m_line_edit->setFocus();
}

void ChatWidget::establishConnect()
{
    connect(m_main_tool, SIGNAL(clicked(bool)), this, SLOT(onMainTool()));
    connect(m_font_combobox, SIGNAL(currentFontChanged(const QFont &)),
            this, SLOT(onFontCombobox(const QFont &)));
    connect(m_size_combobox, SIGNAL(currentTextChanged(const QString &)),
            this, SLOT(onSizeCombobox()));
    connect(m_color_tool, SIGNAL(clicked()), this, SLOT(onColorTool()));
    connect(m_bold_tool, SIGNAL(clicked(bool)), this, SLOT(onBoldTool(bool)));
    connect(m_underline_tool, SIGNAL(clicked(bool)), this, SLOT(onUnderTool(bool)));
    connect(m_italic_tool, SIGNAL(clicked(bool)), this, SLOT(onItalicTool(bool)));
    connect(m_history_push, SIGNAL(clicked()), this, SLOT(onHistoryPush()));
    /*文本编辑控件按回车键触发信号*/
    connect(m_line_edit, SIGNAL(returnPressed()), this, SLOT(onLineEdit()));
    connect(m_send_push, SIGNAL(clicked()), this, SLOT(onSendPush()));
}

void ChatWidget::writeTextBrower(const QString &name, const QString &msg)
{
    //得到当前时间，并把时间格式化为"yyyy-MM-dd hh:ss:ss"形式的字符串
    QString title = QString(DIALOGUE_FORMAT).arg(name).arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"));
    QFont font(m_text_brower->currentFont());
    m_text_brower->setCurrentFont(QFont("楷体",15));
    if (m_send_name == name)
    {
        m_text_brower->setTextColor(Qt::black);
    }
    else
    {
        m_text_brower->setTextColor(Qt::magenta);
    }
    m_text_brower->setFontWeight(QFont::Bold);
    m_text_brower->append(title);
    m_text_brower->setFontWeight(QFont::Normal);
    m_text_brower->setTextColor(*m_color);
    m_text_brower->setCurrentFont(font);
    m_text_brower->setFontPointSize(m_size_combobox->currentText().toDouble());
    m_text_brower->append(msg);
    //QString str = QString(COLOR_FONT).arg(msg);
    //QColor  clrR(255,0,0);
    //stringToHtmlFilter(str);
    //stringToHtml(str,clrR);
    //m_text_brower->insertHtml(str);
    //QString imgPath = QString(":/image/245600445.jpg");
    //QString imgPath = QString("E:/2017-12-12_111210.png");
    //imgPathToHtml(imgPath);
    //m_text_brower->insertHtml(imgPath);
    /*当消息浏览器中消息过多出现滚动条时，自动滚动到最下方*/
    m_text_brower->verticalScrollBar()->setValue(m_text_brower->verticalScrollBar()->maximum());
}

#if 0
QFile file("file.html");
if(!file.open(QFile::ReadOnly | QFile::Text))
    qDebug() << "Can not open";
QTextStream in(&file);
licenceTextBrowser->setHtml(in.readAll());
#endif

#if 0
void ChatWidget::stringToHtmlFilter(QString &str)
{
    //注意这几行代码的顺序不能乱，否则会造成多次替换
    str.replace("&","&amp;");
    str.replace(">","&gt;");
    str.replace("<","&lt;");
    str.replace("\"","&quot;");
    str.replace("\'","&#39;");
    str.replace(" ","&nbsp;");
    str.replace("\n","<br>");
    str.replace("\r","<br>");
}

void ChatWidget::stringToHtml(QString &str,QColor crl)
{
     QByteArray array;
     array.append(crl.red());
     array.append(crl.green());
     array.append(crl.blue());
     QString strC(array.toHex());
     str = QString("<span style=\" color:#%1;\">%2</span>").arg(strC).arg(str);
 }
#endif

void ChatWidget::imgPathToHtml(QString &path)
{
     path = QString("<img src=\"%1\" height=\"200\" width=\"200\" />").arg(path);
}

void ChatWidget::onMainTool()
{
    m_handle->hide();
    m_handle->showNormal();
    this->hide();
}

void ChatWidget::onFontCombobox(const QFont &font)
{
    m_text_brower->setCurrentFont(font);
    m_text_brower->setFontPointSize(m_size_combobox->currentText().toDouble());
    /*判断tool控件是否已是按下状态*/
    if(m_bold_tool->isChecked())
        m_text_brower->setFontWeight(QFont::Bold);
    else
        m_text_brower->setFontWeight(QFont::Normal);

    if(m_italic_tool->isChecked())
        m_text_brower->setFontItalic(true);
    else
        m_text_brower->setFontItalic(false);

    if(m_underline_tool->isChecked())
        m_text_brower->setFontUnderline(true);
    else
        m_text_brower->setFontUnderline(false);

    m_text_brower->setTextColor(*m_color);
    m_line_edit->setFocus();
}

void ChatWidget::onSizeCombobox()
{
    m_text_brower->setFontPointSize(m_size_combobox->currentText().toDouble());
    m_line_edit->setFocus();
}

void ChatWidget::onColorTool()
{
    *m_color = QColorDialog::getColor();
    if (m_color->isValid())
        m_text_brower->setTextColor(*m_color);
    m_line_edit->setFocus();
}

void ChatWidget::onBoldTool(bool check)
{
    if(check)
    {
        m_text_brower->setFontWeight(QFont::Bold);
        m_bold_tool->setChecked(true);
    }
    else
        m_text_brower->setFontWeight(QFont::Normal);
    m_line_edit->setFocus();
}

void ChatWidget::onUnderTool(bool check)
{
    if(check)
    {
        m_text_brower->setFontUnderline(true);
        m_underline_tool->setChecked(true);
    }
    else
        m_text_brower->setFontUnderline(false);
    m_line_edit->setFocus();
}

void ChatWidget::onItalicTool(bool check)
{
    if(check)
    {
        m_text_brower->setFontItalic(true);
        m_italic_tool->setChecked(true);
    }
    else
        m_text_brower->setFontItalic(false);
    m_line_edit->setFocus();
}

void ChatWidget::onHistoryPush()
{

}

void ChatWidget::onLineEdit()
{
    if (m_send_push->isEnabled())
        onSendPush();
}

void ChatWidget::onSendPush()
{
    m_array->clear();
    if (m_line_edit->text().length() > MAX_LINE_EDIT_BYTES)
    {
        m_text_brower->append(SEND_FAILED);
    }
    else
    {
        /*在TCP传输的数据中,需要转换为UTF8格式,兼容ubuntu的编码格式*/
        m_array->append(m_line_edit->text().toUtf8());
        writeTextBrower(m_send_name, *m_array);
        /*用户为在线状态才发送消息*/
        if (m_status == ONLINE)
        {
            m_handle->sendMsgToFriend(m_id, *m_array);
        }
        else if (m_status == LEAVE)
        {
            writeTextBrower(m_recv_name, QByteArray(SEND_AUTO_MSG));
        }
    }
    m_line_edit->clear();
    m_line_edit->setFocus();
}
