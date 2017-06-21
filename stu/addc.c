#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "cgic.h"

char * headname = "head.html";
char * footname = "footer.html";

int cgiMain()
{
	FILE * fd;

	fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");

	char cname[32] = "\0";
	char cId[32] = "\0";
	char credit[8] = "\0";
	int status = 0;
	char ch;

	if(!(fd = fopen(headname, "r"))){
		fprintf(cgiOut, "Cannot open file, %s\n", headname);
		return -1;
	}
	ch = fgetc(fd);

	while(ch != EOF){
		fprintf(cgiOut, "%c", ch);
		ch = fgetc(fd);
	}
  fclose(fd);

	status = cgiFormString("cname",  cname, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get cname error!\n");
		return 1;
	}

	status = cgiFormString("cId",  cId, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get cId error!\n");
		return 1;
	}

	status = cgiFormString("credit",  credit, 8);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get credit error!\n");
		return 1;
	}
	//fprintf(cgiOut, "name = %s, age = %s, stuId = %s, sex = %s\n", name, age, stuId, sex);

	int ret;
	char sql[128] = "\0";
	MYSQL *db;

	//初始化
	db = mysql_init(NULL);
	mysql_options(db,MYSQL_SET_CHARSET_NAME,"utf8");
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_init fail:%s\n", mysql_error(db));
		return -1;
	}

	//连接数据库
	db = mysql_real_connect(db, "127.0.0.1", "root", "123456", "stu",  3306, NULL, 0);
	if (db == NULL)
	{
		fprintf(cgiOut,"mysql_real_connect fail:%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}


  //mysql_query(db, "set character set utf8");
	strcpy(sql, "create table course(cid int(4) primary key check(cid>0), cname varchar(20) not null, credit int(2) not null check(credit>0))default charset=utf8");
	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		if (ret != 1)
		{
			fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
			mysql_close(db);
			return -1;
		}
	}



	sprintf(sql, "insert into course values(%d, '%s', %d)", atoi(cId), cname, atoi(credit));
	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		fprintf(cgiOut, "%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">添加课程成功</h1>");
	
	mysql_close(db);
	return 0;
}
