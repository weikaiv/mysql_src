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

	char cId[32] = "\0";
	char stuId[32] = "\0";
	char sgrade[8] = "\0";
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

	status = cgiFormString("cId",  cId, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get cId error!\n");
		return 1;
	}

	status = cgiFormString("stuId",  stuId, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get stuId error!\n");
		return 1;
	}

	status = cgiFormString("sgrade",  sgrade, 8);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get sgrade error!\n");
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
	strcpy(sql, "create table score(id int(4) check(id>0), cid int(4) check(id>0), sgrade double(4,1) default'0' check(sid>=0 and sgrade<=100), primary key(id,cid), foreign key(id) references information(id), foreign key(cid) references course(cid))default charset=utf8");
	if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	{
		if (ret != 1)
		{
			fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
			mysql_close(db);
			return -1;
		}
	}



	sprintf(sql, "update score set sgrade=%.1f where id=%d and cid=%d", strtod(sgrade,NULL), atoi(stuId), atoi(cId));
	if (mysql_real_query(db, sql, strlen(sql) + 1) != 0)
	{
		fprintf(cgiOut, "%s\n", mysql_error(db));
		mysql_close(db);
		return -1;
	}

	fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">录入学生成绩成功</h1>");

	mysql_close(db);
	return 0;
}
