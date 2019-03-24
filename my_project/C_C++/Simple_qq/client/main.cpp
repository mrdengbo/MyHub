#include <QApplication>
#include "maindialog_2.h"
//#include "edutdialog.h"
//#include <QPixmap>
//#include <QString>

QTcpSocket *MainDialog_2::s_socket = NULL;
int MainDialog_2::s_id      = 0;
int MainDialog_2::s_peer_id = 0;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //RegisterDialog r;
    //r.show();
    //ConnectNetworkDialog c;
    //c.show();
    //EdutDialog e(QPixmap(":/image/default.png"), "帅哥", "寂寞无痕");
    //e.show();
    //ToolButtons t;
    /*固定主界面大小*/
    //t.setFixedSize(QSize(MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT));
    //t.show();
    MainDialog_2 m;

    m.show();
    return a.exec();
}
