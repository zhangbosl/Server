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
        MYSQL mysql;
        MYSQL_RES *result = NULL;
        MYSQL_ROW row;
        
        char sqlStr[1024]={0};
        
        mysql_init(&mysql);
        if(mysql_real_connect(&mysql,"localhost","root",NULL,"chat",0,NULL,0)==NULL)
        {
            printf("connect error: %s\n",mysql_error(&mysql));
            return ;
        }
        else
            printf("connected\n");

        char buf2[1024]={0},buf3[1024]={0},buf4[1024]={0},buf5[1024]={0};

        sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]%*[|]%[^|]",buf2,buf3,buf4,buf5);
        //printf("%s\n%s\n%s\n%s\n",buf2,buf3,buf4,buf5);
        
        sprintf(sqlStr,"insert into user values(NULL,'%s','%s','%s',0)",buf2,buf3,buf4);
        //printf("sqlStr=%s\n",sqlStr);
        
        if(mysql_query(&mysql,sqlStr) != 0)
        {
            printf("query error: %s\n",mysql_error(&mysql));
            return ;
        }
        
        sprintf(sqlStr,"select * from user order by id desc limit 1");
        //printf("sqlStr=%s\n",sqlStr);
        
        if(mysql_query(&mysql,sqlStr) != 0)
        {
            printf("query error: %s\n",mysql_error(&mysql));
            return ;
        }
        
        result=mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        if(row)
        {
            write(confd,"#001|0|regSucess",16);
            MyFd.clifd[i].second=row[0];
            //std::cout<<MyFd.clifd[i].second<<std::endl;
        }
        else
        {
            write(confd,"#001|1|regFailed",16);
        }

        mysql_free_result(result);
        mysql_close(&mysql);
	}
	else if(num1 == 2)
	{
	
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%s",buf1,buf2,buf3);
		strcat(str,"select * from user where id = '");
		strcat(str,buf2);
		strcat(str,"' and password = '");
		strcat(str,buf3);
		strcat(str,"';");
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		//sucess
		if(row)
		{
		
			write(confd,"0",1);
			MyFd.clifd[i].second = buf2;
			memset(str,0,sizeof(str));
			strcat(str,"update user set online = 1 where id = '");
			strcat(str,row[0]);
			//printf("%s\n",row[0]);
			strcat(str,"';");
			mysql_query(&mysql,str);
			//mysql_query(&mysql,);
		}
		else
		{
			write(confd,"-1",2);
		}
	}

	
	/*
	
	
	
	write(confd,"ok",2);
	*/
	
}
//g++ MyServer.cpp Init.cpp FD.cpp WorkData.cpp -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
