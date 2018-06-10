#ifndef EDUTDIALOG_H
#define EDUTDIALOG_H

#include <QDialog>
#include <QString>

namespace Ui {
class EdutDialog;
}

class EdutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EdutDialog(const QPixmap &pix, const QString &name, const QString &indiv, QWidget *parent = 0);
    ~EdutDialog();

    QString m_name;
    QString m_indiv;
    QPixmap m_pixmap;
    bool isClick;
    bool hasLoad;

private slots:
    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::EdutDialog *ui;
};

#endif // EDUTDIALOG_H
