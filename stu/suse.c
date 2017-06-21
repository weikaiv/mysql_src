#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include "cgic.h"

char * headname = "head.html";
char * sindex ="sindex.html";
char * footname = "footer.html";

int cgiMain()
{
	FILE * fd;

	fprintf(cgiOut, "Content-type:text/html;charset=utf-8\n\n");

  char stuId[32] = "\0";
  int status = 0;
	char ch;

  status = cgiFormString("stuId",  stuId, 32);
	if (status != cgiFormSuccess)
	{
		fprintf(cgiOut, "get stuId error!\n");
		return 1;
	}

  int ret;
	char sql[128] = "\0";
	MYSQL *db;

	//初始化
	db = mysql_init(NULL);
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
	if (res = NULL){
		fprintf(cgiOut,"mysql_real_query fail:%s\n", mysql_error(db));
		return -1;
	}
	if (num)
	{

		if(!(fd = fopen(sindex, "r"))){
			fprintf(cgiOut, "Cannot open file, %s\n", sindex);
			return -1;

		}
		ch = fgetc(fd);

		while(ch != EOF){
			fprintf(cgiOut, "%c", ch);
			ch = fgetc(fd);
		}
	  fclose(fd);
	}

	// fprintf(cgiOut, "delete stu ok!\n");
	mysql_close(db);

	return 0;
}
