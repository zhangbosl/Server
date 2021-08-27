#include "WorkData.h"


void WorkData(int confd,int i,FD &MyFd)
{
	
	char buf[1024]={0};
	
	//断开连接
	if(!read(confd,buf,sizeof(buf)))
	{
		//allset和clifd中删除
		FD_CLR(confd,&MyFd.allset);
		MyFd.clifd.erase(MyFd.clifd.begin()+i);
		return ;
	}
	//如果读入了数据在buf中
	printf("buf=%s\n",buf);
	char buf1[1024]={0},buf2[1024]={0},buf3[1024]={0},str[1024]={0};
	
	sscanf(buf,"%[^|]",buf1);
	int num1=atoi(buf1);
	
	
	MYSQL mysql;
	MYSQL_RES *result=NULL;
	MYSQL_ROW row;
	mysql_init(&mysql);
	mysql_real_connect(&mysql,"127.0.0.1","root","root","chat",0,NULL,0);
	
	
	if(num1 == 1)
	{
		
	
	}
	else if(num1 == 2)
	{
	
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%s",buf1,buf2,buf3);
		strcat(str,"select * from user where id =");
		strcat(str,buf2);
		strcat(str," and password =");
		strcat(str,buf3);
		strcat(str,";");
		mysql_query(&mysql,str);
		result=mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			write(confd,"-1",2);
		}
		else
		{
			write(confd,"0",1);
			MyFd.clifd[i].second = buf2;
			
			mysql_query(&mysql,"update user set online = 1;");
			
		}
	}

	
	/*
	
	
	//mysql_query(&mysql,"INSERT INTO Table_User(account,password)VALUES('张三','0001'),('孙六','0004');");

	
	if(result==NULL)
	{
		printf("result error\n");
	}
	row = mysql_fetch_row(result);

	if(row)
	{
		printf("yong hu yi cun zai");
	}
	
	write(confd,"ok",2);
	*/
	
}
//g++ MyServer.cpp Init.cpp FD.cpp WorkData.cpp -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
