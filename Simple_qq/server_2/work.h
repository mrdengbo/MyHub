#ifndef __WORK_H__
#define __WORK_H__

#define IPC_FILE "./"

#define DATA_SIZE     2048
#define BUF_SIZE      2048
#define SEQLIST_SIZE  100

#define WORK_SUCCESS  0
#define WORK_FAILURE -1

#define DATAGRAM_HEAD sizeof(int)
#define DATAGRAM_MSG sizeof(MsgControl)
#define DATAGRAM_AP sizeof(AccountPasswd)
#define DATAGRAM_USER sizeof(UserInfo)

#define ACCOUNT_ERR_STR   "账号不存在"
#define AP_ERR_STR				"账号密码不匹配"
#define USER_ONLINE_ERR_STR  "用户已经在线"
#define STATUS_ERR_STR    "登录状态出错"
#define LOAD_ERR_STR      "载入用户信息失败"
#define USER_ERR_STR      "用户信息异常"
#define CONFIG_ERR_STR    "配置出错"
 
#define SEND_MSG_ERR_STR  "消息发送失败"
 
#define ACCOUNT_EXIST_ERR_STR "账号已存在"
#define REGISTER_ERR_STR 			"注册异常"

#define UPDATE_NICKNAME_ERR_STR "昵称修改失败"
#define UPDATE_INDIV_ERR_STR 		"个性签名修改失败"

#define QUERY_ONLINE_FRIEND_ERR_STR		"搜索在线好友失败"
#define GET_FRIEND_OF_USER_ERR_STR    "获取我的好友失败"

#define ADD_NEW_FRIEND_ERR_STR   "添加好友失败"
#define DELIVER_MSG_ERR_STR      "传达消息时出错"

#define DEFAULT_NICKNAME "默认用户"
#define DEFAULT_INDIV "此人很懒，没有个性签名。"

#define ONLINE 1  /*在线*/
#define LEAVE  0  /*离线*/

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
    REQUEST_ADD_FRIEND    /*请求添加好友*/
    //TODO
};

/*定义添加好友消息机制*/
enum AddFriend {
	REQUEST = 100 ,  /*发送请求*/
	AGREEMENT     ,  /*同意*/
	REFUSE           /*拒绝*/
};

/*定义流程处理错误类型*/
enum ClientError {
	INVALID_ARG  , /*无效参数(不处理)*/
	ACCOUNT_ERR  , /*账号不存在*/
	AP_ERR       , /*账号密码不匹配*/
	USER_ONLINE_ERR ,  /*用户已经在线*/
	STATUS_ERR   , /*设置状态错误*/
	LOAD_ERR     , /*载入用户信息失败*/
	USER_ERR     , /*用户信息异常*/
	CONFIG_ERR   , /*配置出错*/
	
	SEND_MSG_ERR , /*消息发送失败*/
	
	ACCOUNT_EXIST_ERR , /*账号已存在*/ 
	REGISTER_ERR      , /*注册异常*/
	
	UPDATE_NICKNAME_ERR , /*昵称修改失败*/
	UPDATE_INDIV_ERR    , /*个性签名修改失败*/
	
	QUERY_ONLINE_FRIEND_ERR	 ,	/*搜索在线好友失败*/
	GET_FRIEND_OF_USER_ERR   ,  /*获取我的好友失败*/
	
	ADD_NEW_FRIEND_ERR  , /*添加好友失败*/
	DELIVER_MSG_ERR       /*传达消息时出错*/
	//TODO
};

/*定义消息处理结果类型*/
enum ClientResult {
	CLIENT_SUCCESS = 0   , /*成功*/
	CLIENT_FAILURE       , /*失败*/
	CLIENT_RECV_TIMEOUT  , /*接收超时*/
	CLIENT_SEND_TIMEOUT    /*发送超时*/
	//TODO
};

/*定义客户端/服务器消息通信机制数据类型*/
typedef struct _MsgControl {
	unsigned char client_request; /*客户端请求*/
	unsigned char server_reply;   /*服务端回复*/
	unsigned char head_1;					/*控制位1*/
	unsigned char head_2;					/*控制位2*/
} MsgControl;

/*定义参数传递结构数据类型*/
typedef struct _Parameter {
	void         *handle;  	/*服务器通信句柄*/
	int 	        sockfd;		/*客户端socket*/
	void         *data;		 	/*通信数据内容*/
	int 	        length;		/*通信数据字节数*/
} Parameter;

/*定义账号和密码数据类型*/
typedef struct _AccountPasswd {
  char account[12];  /*账号*/
  char passwd[24];	 /*密码*/
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

void doClient(void *handle, int sockfd, const char *ip);

#endif /*__WORK_H__*/