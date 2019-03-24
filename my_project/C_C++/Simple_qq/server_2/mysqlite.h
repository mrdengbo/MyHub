#ifndef __MYSQLITE_H__
#define __MYSQLITE_H__

#define SQL_BUF_SIZE 1024
#define SQLITE_SUCCESS 0
#define SQLITE_FAILURE -1

enum UpdateOption {
	RESET_STATUS = 0,  /*置0登录状态(不在线)*/
	UPDATE_STATUS   ,  /*置1登录状态(在线)*/
	UPDATE_PASSWD   ,  /*修改密码*/
	UPDATE_NICKNAME ,  /*修改昵称*/
	UPDATE_INDIV    ,  /*修改个性签名*/
	UPDATE_LAST     ,  /*更新登录时间*/
	DELETE_USER     ,  /*删除用户*/
	DELETE_FRIEND      /*删除好友*/
};

#define INSERT_NEW_USER "insert into users values (null, '%s', '%s', '%s', '%s', '%s', '%s', 0)"
#define CLEAR_USERS_STATUS "update users set status = 0"
#define SELECT_USER "select * from users where account = '%s' and passwd = '%s'"
#define SELECT_ID_BY_ACCOUNT "select id from users where account = '%s'"
#define SELECT_USER_STATUS "select status from users where id = %d"
#define RESET_USER_STATUS "update users set status = 0 where id = %d"
#define UPDATE_USER_STATUS "update users set status = 1 where id = %d"
#define SELECT_USER_INFO "select account, passwd, nickname, individuality, register, last, status from users where id = %d"
#define	UPDATE_PASSWD_BY_ID "update users set passwd = '%s' where id = %d"
#define UPDATE_NICKNAME_BY_ID "update users set nickname = '%s' where id = %d"
#define UPDATE_INDIV_BY_ID "update users set individuality = '%s' where id = %d"
#define UPDATE_LAST_BY_ID "update users set last = '%s' where id = %d"
/*查询本用户的所有好友id*/
#define SELECT_FRIENDS "select fid from friend where id = %d"
#define DELETE_USER_BY_ID "delete from users where id = %d"
#define DELETE_FRIEND_RELATION "delete from friend where id = %d or fid = %d"
/*查询所有在线非本用户的好友id*/
#define SELECT_ONLINE_FRIEND "select id from users where status = 1 and id != %d"
/*添加新好友*/
#define INSERT_NEW_FRIEND "insert into friend values (%d, %d)"
#define SELECT_NEW_FRIEND "select * from friend where id = %d and fid = %d"

void openMysqlite(const char *file);
void closeMysqlite();
int clearUsersStatus();
int insertNewUser(const char *account, const char *passwd, const char *nickname, 
									const char *indiv, const char *timestr);
int isExistUser(const char *account, const char *passwd);
int getUserId(const char *account);
int getUserStatus(int id);
int setUserStatus(int id, int flg);
int getUserInfo(int id, char ***result, int *row, int *column);
int setUserInfo(int id, const char *sqlstr, int flg);
int getFriendsId(int id, char ***result, int *row, int *column);
int deleteUser(int id, int flg);
int selectOnlineFriend(int id, char ***result, int *row, int *column);
int insertNewFriend(int id, int new_id);

	
#endif /*__MYSQLITE_H__*/
