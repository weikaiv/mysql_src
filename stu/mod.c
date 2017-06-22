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

	char name[32] = "\0";
	char age[16] = "\0";
	char stuId[32] = "\0";
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

	status = cgiFormString("name",  name, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get name error!\n");
		return 1;
	}

	status = cgiFormString("age",  age, 16);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get age error!\n");
		return 1;
	}

	status = cgiFormString("stuId",  stuId, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get stuId error!\n");
		return 1;
	}

	//fprintf(cgiOut, "name = %s, age = %s, stuId = %s\n", name, age, stuId);

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

  sprintf(sql, "select * from information where id =%d", atoi(stuId));

  if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
 {
   fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
   mysql_close(db);
   return -1;
 }

  MYSQL_RES *res;
  res = mysql_store_result(db);
  int num= (int)res->row_count;

  if(num){
   sprintf(sql, "update information set name='%s', age= %d where id = %d ", name, atoi(age), atoi(stuId));

   if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
 	{
 		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
 		mysql_close(db);
 		return -1;
 	}

   fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">修改学生信息成功</h1>");
 }else{
   fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">您输入的学号不存在，请重新输入！</h1>");
 }

	// sprintf(sql, "update information set name='%s', age= %d where id = %d ", name, atoi(age), atoi(stuId));
	// if ((ret = mysql_real_query(db, sql, strlen(sql) + 1)) != 0)
	// {
	// 	fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
	// 	mysql_close(db);
	// 	return -1;
	// }
  //
  // fprintf(cgiOut, "<div class=\"container\"> <h1 class=\"text-center\">修改学生信息成功</h1>");

	mysql_close(db);
	return 0;
}
