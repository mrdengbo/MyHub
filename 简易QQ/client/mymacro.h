#ifndef MYMACRO_H
#define MYMACRO_H

#define DEBUG_OUT qDebug() << __FILE__ << __FUNCTION__ << __LINE__

#define TITLE_TEXT      "xiaoma_QQ"
#define IMAGE_NAME_JPG  ":/image/%1.jpg"
#define IMAGE_NAME_PNG  ":/image/%1.png"
#define TOOL_BUTTON_IMG ":/image/default.png"
#define MAIN_TOOL_IMG   ":/image/main.jpg"
#define MY_JPG          ":/image/245600445.jpg"
#define XIAOMA_ACCOUNT  "245600445"
#define PER_SECOND      "%1 bytes/s"

#define MAX_LINE_EDIT_BYTES 8196
#define SEND_FAILED "字节数超过8196，发送失败。"

#define MY_JPG_WIDTH  70
#define MY_JPG_HEIGHT 60

#define ONLINE 1  /*在线*/
#define LEAVE  0  /*离线*/

#define ONLINE_FRIEND_BUTTON_TIP "%1(在线)"
#define LEAVE_FRIEND_BUTTON_TIP  "%1(离线)"
#define QUERY_DIALOG_BUTTON_TIP  "点我! 点我! 点我!"
#define CLICK_ADD_FRIEND "点击添加好友"
#define REQUEST_FRIEND   "昵称：%1\n账号：%2\n请求添加好友!"
#define AGREEMENT_FRIEND "昵称：%1\n同意添加您为好友,现在可以与他(她)聊天哦!"
#define REFUSE_FRIEND    "昵称：%1\n拒绝添加您为好友!"
#define DELETE_TIP       "年轻人，确定要放弃治疗吗?"
#define LOGIN_TIP        "欢迎%1用户再次回归，上次登陆时间为:\n%2"
#define LOAD_IMG_TIP     "没卵用，头像上传功能未完善!"
#define HISTORY_TIP      "放弃。"
#define EDIT_IMG_TIP     "花里胡哨"
#define SEND_AUTO_MSG    "对方不想接收你的消息, 并向你扔了一段BUG信息\nSegmentation fault (core dumped)"
#define TITLE_DESC       "%1(%2)\n%3"
#define DIALOGUE_FORMAT  "%1 %2"
#define COLOR_FONT       " < %1!>\n"
#define NAME_LABEL       "%1(%2)"
#define HTML_FONT        "QLabel {font:bold 13px; color:purple; padding:5px}"

#define TEXT_BROWSER_STYLE_SHEET "background-image: url(:/image/chat_brower.jpg);"

#define CHAT_WIDTH  640
#define CHAT_HEIGHT 560

#define MAIN_WINDOW_WIDTH   250
#define MAIN_WINDOW_HEIGHT  600

#define MARGIN_SIZE        10
#define TOOL_BUTTON_WIDTH  210
#define TOOL_BUTTON_HEIGHT 48
#define TOOL_ICON_WIDTH    48
#define TOOL_ICON_HEIGHT   48

#define DATAGRAM_HEAD sizeof(int)
#define DATAGRAM_MSG  sizeof(MsgControl)
#define DATAGRAM_AP   sizeof(AccountPasswd)
#define DATAGRAM_USER sizeof(UserInfo)

#define MAX_BUF_SIZE 2048
#define DATA_SIZE 1024
#define AVAILABLE_SIZE (MAX_BUF_SIZE - DATAGRAM_HEAD - DATAGRAM_MSG)

/*定义客户端/服务器消息通信机制数据类型*/
typedef struct _MsgControl {
    unsigned char client_request; /*客户端请求*/
    unsigned char server_reply;   /*服务端回复*/
    unsigned char head_1;		  /*控制位1*/
    unsigned char head_2;		  /*控制位2*/
} MsgControl;

typedef struct _AccountPasswd {
    char account[12];  /*账号*/
    char passwd[24];   /*密码*/
} AccountPasswd;

/*定义当前客户端的用户信息结构类型*/
typedef struct _UserInfo {
    AccountPasswd ap;  /*账号和密码*/
    int  id;           /*唯一ID*/
    char name[24];     /*昵称*/
    char indiv[100];   /*个性签名*/
    char regis[32];    /*注册日期*/
    char last[32];     /*上次登录日期*/
    int  status;       /*登录状态*/
} UserInfo;

/*定义消息通信的主处理流程类型*/
enum ClientMsg {
    LOGIN = 0          ,  /*登录*/
    REGISTER           ,  /*注册*/
    LOGOUT             ,  /*注销*/
    REFRESH_USER_INFO  ,  /*刷新用户信息*/
    REFRESH_FRIEND_INFO,  /*刷新好友信息*/
    MSG_SEND      	   ,  /*消息发送*/
    UPDATE_USER_INFO   ,  /*修改用户信息*/
    DELETE_USER_INFO   ,  /*删除用户信息*/
    QUERY_ONLINE_FRIEND,  /*查询在线好友*/
    REQUEST_ADD_FRIEND ,  /*请求添加好友*/
    UPLOAD_PICTURE_FILE   /*上传图片文件*/
    //TODO
};

/*定义添加好友消息机制*/
enum AddFriend {
    REQUEST = 100 ,  /*发送请求*/
    AGREEMENT     ,  /*同意*/
    REFUSE           /*拒绝*/
};

/*定义消息处理结果类型*/
enum ClientResult {
    CLIENT_SUCCESS = 0   , /*成功*/
    CLIENT_FAILURE       , /*失败*/
    CLIENT_RECV_TIMEOUT  , /*接收超时*/
    CLIENT_SEND_TIMEOUT    /*发送超时*/
    //TODO
};

#endif // MYMACRO_H
