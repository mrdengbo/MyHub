#include "edutdialog.h"
#include "ui_edutdialog.h"
#include "mymacro.h"
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QPixmap>
#include <QMessageBox>
#include <QDebug>

EdutDialog::EdutDialog(const QPixmap &pix, const QString &name, const QString &indiv, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EdutDialog)
{
    ui->setupUi(this);
    /*无边框*/
    setWindowFlags(Qt::FramelessWindowHint);
    /*无任务栏*/
    setWindowFlags(windowFlags() | Qt::Tool);
    ui->label->setToolTip(EDIT_IMG_TIP);
    isClick = false;
    hasLoad = false;
    ui->lineEdit->setText(name);
    ui->lineEdit_2->setText(indiv);
    m_pixmap = pix;
    ui->label->setPixmap(pix);
    ui->label->show();
    ui->lineEdit->setFocus();
}

EdutDialog::~EdutDialog()
{
    delete ui;
}

/*点击上传*/
void EdutDialog::on_pushButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                      "E/",
                                                      tr("Images (*.png *.xpm *.jpg)"));
    /*如果用户未选择图片,则载入原先图片*/
    if (file_name.isEmpty())
    {
        ui->label->setPixmap(m_pixmap);
        ui->label->show();
        hasLoad = false;
    }
    else
    {
        QPixmap pixmap(file_name);
        ui->label->setPixmap(QPixmap(file_name));
        ui->label->show();
        m_pixmap = pixmap;
        hasLoad = true;
    }

#if 0
    QString file_content;
    QFile file(file_name);
    if (file.open(QFile::ReadOnly))
    {
        /*将文件内容载入文本流对象中*/
        QTextStream stream(&file);
        while(!stream.atEnd())
        {
            file_content.append(stream.readLine());
            //qDebug() << stream.readAll();
            /*全部读取内容,写入文本编辑框中*/
            //ui->textEdit->setText(stream.readAll());
        }
        file.close();
    }
#endif
}

/*确认修改*/
void EdutDialog::on_pushButton_3_clicked()
{
    /*将编辑栏的内容写入字符串*/
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox::warning(this, tr("错误"), tr("昵称不能为空"));
        ui->lineEdit->setFocus();
    }
    else if (ui->lineEdit->text().length() > 24)
    {
        QMessageBox::warning(this, tr("错误"), tr("昵称长度不得大于24个字符"));
        ui->lineEdit->setFocus();
    }
    else
    {
        if (ui->lineEdit_2->text().isEmpty())
        {
            QMessageBox::warning(this, tr("错误"), tr("个性签名不能为空"));
            ui->lineEdit->setFocus();
        }
        else
        {
            isClick = true;
            m_name = ui->lineEdit->text();
            m_indiv = ui->lineEdit_2->text();
            /*关闭注册窗口*/
            close();
        }
    }
}

/*退出编辑*/
void EdutDialog::on_pushButton_2_clicked()
{
    close();
}
