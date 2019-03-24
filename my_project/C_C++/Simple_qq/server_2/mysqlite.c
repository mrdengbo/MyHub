#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sqlite3.h>

#include "mysqlite.h"

/*定义数据库句柄*/
sqlite3 *handle = NULL;
/*定义数据库出错句柄*/
char *error_msg = NULL;

void openMysqlite(const char *file)
{
    /*打开数据库文件*/
    if (sqlite3_open(file, &handle) != SQLITE_OK)
	{
    	fprintf(stderr, "open failed, %s\n", sqlite3_errmsg(handle));
		exit(EXIT_FAILURE);
	}
	printf("success open sqlite\n");
	return ;
}

void closeMysqlite()
{
    /*关闭数据库文件*/
    sqlite3_close(handle);
	printf("success close sqlite\n");
	return ;
}

/*执行非select语句处理数据*/
static int dealData(const char *sql)
{
    if (sqlite3_exec(handle, sql, NULL, NULL, &error_msg) != SQLITE_OK)
    {
        fprintf(stderr, "deal exec failed, %s\n", error_msg);
        return SQLITE_FAILURE;
    }
    return SQLITE_SUCCESS;
}

/*select查询数据*/
static int selectData(const char *sql, char ***res, int *row, int *column)
{
    if (sqlite3_get_table(handle, sql, res, row, column, &error_msg) != SQLITE_OK)
    {
        fprintf(stderr, "select exec failed, %s\n", error_msg);
        return SQLITE_FAILURE;
    }
	return SQLITE_SUCCESS;
}

/*重置所有用户登录状态*/
int clearUsersStatus()
{
	return dealData(CLEAR_USERS_STATUS);
}

/*插入新用户数据*/
int insertNewUser(const char *account, const char *passwd, const char *nickname, 
									const char *indiv, const char *timestr)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, INSERT_NEW_USER, account, passwd, nickname, indiv, timestr, timestr);
	return dealData(buf);
}

/*查询用户是否存在, 存在返回0，不存在返回-1*/
int isExistUser(const char *account, const char *passwd)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_USER, account, passwd);
  char **result = NULL;
  int row, column;
	if (selectData(buf, &result, &row, &column))
		return SQLITE_FAILURE;
	if (column == 0)
		return SQLITE_FAILURE;
	return SQLITE_SUCCESS;
}

/*根据账户名获取用户ID,若成功返回用户ID,失败则返回-1*/
int getUserId(const char *account)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_ID_BY_ACCOUNT, account);
  char **result = NULL;
  int row, column;
  int index = 0;
	if (selectData(buf, &result, &row, &column)) {
		return SQLITE_FAILURE;
	}
	if (column == 0) {
		return SQLITE_FAILURE;
	}
	index = column;
	/*ID字符串字段转化为整型*/
	return atoi(result[index]);
}

/*获取指定用户的登录状态*/
int getUserStatus(int id)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_USER_STATUS, id);
  char **result = NULL;
  int row, column;
  int index = 0;
	if (selectData(buf, &result, &row, &column)) {
		return SQLITE_FAILURE;
	}
	if (column == 0) {
		return SQLITE_FAILURE;
	}
	index = column;
	/*ID字符串字段转化为整型*/
	return atoi(result[index]);
}

/*根据ID与标志位重置用户登录状态*/
int setUserStatus(int id, int flg)
{
	char buf[SQL_BUF_SIZE] = {0};
	if (flg == UPDATE_STATUS) {
		sprintf(buf, UPDATE_USER_STATUS, id);
	}
	else if (flg == RESET_STATUS) {
		sprintf(buf, RESET_USER_STATUS, id);
	}
	else {
		return SQLITE_FAILURE;
	}
	return dealData(buf);
}

/*根据ID获取用户有效信息*/
int getUserInfo(int id, char ***result, int *row, int *column)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_USER_INFO, id);
	return selectData(buf, result, row, column);
}

/*设置指定用户信息*/
int setUserInfo(int id, const char *sqlstr, int flg)
{
	char buf[SQL_BUF_SIZE] = {0};
	if (flg == UPDATE_PASSWD) {
		sprintf(buf, UPDATE_PASSWD_BY_ID, sqlstr, id);
	}
	else if (flg == UPDATE_NICKNAME) {
		sprintf(buf, UPDATE_NICKNAME_BY_ID, sqlstr, id);
	}
	else if (flg == UPDATE_INDIV) {
		sprintf(buf, UPDATE_INDIV_BY_ID, sqlstr, id);
	}
	else if (flg == UPDATE_LAST) {
		sprintf(buf, UPDATE_LAST_BY_ID, sqlstr, id);
	}
	else {
		return SQLITE_FAILURE;
	}
	return dealData(buf);
}

/*根据ID获取用户的所有好友ID*/
int getFriendsId(int id, char ***result, int *row, int *column)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_FRIENDS, id);
	return selectData(buf, result, row, column);
}

/*根据ID删除该用户信息*/
int deleteUser(int id, int flg)
{
	char buf[SQL_BUF_SIZE] = {0};
	if (flg == DELETE_USER) {
		sprintf(buf, DELETE_USER_BY_ID, id);
	}
	else if (flg == DELETE_FRIEND) {
		sprintf(buf, DELETE_FRIEND_RELATION, id, id);
	}
	else {
		return SQLITE_FAILURE;
	}
	return dealData(buf);
}

int selectOnlineFriend(int id, char ***result, int *row, int *column)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_ONLINE_FRIEND, id);
	return selectData(buf, result, row, column);
}

int insertNewFriend(int id, int new_id)
{
	char buf[SQL_BUF_SIZE] = {0};
	sprintf(buf, SELECT_NEW_FRIEND, id, new_id);
	char **result = NULL;
  int row, column;
	if (selectData(buf, &result, &row, &column)) {
		return SQLITE_FAILURE;
	}
	if (column == 0) {
		memset(buf, 0, SQL_BUF_SIZE);
		sprintf(buf, INSERT_NEW_FRIEND, id, new_id);
		return dealData(buf);
	}
	return SQLITE_SUCCESS;
}