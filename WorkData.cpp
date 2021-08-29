#include "WorkData.h"


void WorkData(int confd,int i,FD &MyFd)
{
	
	char buf[1024]={0};
	printf("i = %d\n",i);
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
	char buf1[1024]={0},buf2[1024]={0},buf3[1024]={0},buf4[1024]={0},buf5[1024]={0},str[1024]={0},sqlStr[1024]={0};
	
	sscanf(buf,"%[^|]",buf1);
    if(buf1[0]=='#')
        sscanf(buf1,"#%s",buf1);
	int num1=atoi(buf1);
	
	
	MYSQL mysql;
    MYSQL_RES *result = NULL;
    MYSQL_ROW row;
    
    
    mysql_init(&mysql);
    if(mysql_real_connect(&mysql,"localhost","root",NULL,"chat",0,NULL,0)==NULL)
    {
        printf("connect error: %s\n",mysql_error(&mysql));
        return ;
    }
    else
        printf("connected\n");

	
	if(num1 == 1)
	{
        sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]%*[|]%[^|]%*[|]%[^|]\n",buf1,buf2,buf3,buf4,buf5);
        
        sprintf(sqlStr,"insert into user (password,question,answer,online) values('%s','%s','%s',0)",buf3,buf4,buf5);
        if(mysql_query(&mysql,sqlStr) != 0)
        {
            printf("query error: %s\n",mysql_error(&mysql));
            return ;
        }
        
        sprintf(sqlStr,"select * from user order by id desc limit 1");
        if(mysql_query(&mysql,sqlStr) != 0)
        {
            printf("query error: %s\n",mysql_error(&mysql));
            return ;
        }
        
        result=mysql_store_result(&mysql);
        row = mysql_fetch_row(result);
        
        if(row&&row[1]==buf3&&row[2]==buf4&&row[3]==buf5)
        {
            write(confd,"#001|0|regSucess",16);
            MyFd.clifd[i].second=row[0];
        
            sprintf(sqlStr,"insert into uinfor (id,name) values('%s','%s')",row[0],buf1);
            if(mysql_query(&mysql,sqlStr) != 0)
            {
                printf("query error: %s\n",mysql_error(&mysql));
                return ;
            }
        }
        else
        {
            write(confd,"#001|1|regFailed",16);
        }

        mysql_free_result(result);
	}
	else if(num1 == 2)
	{
		//buf2 ->id buf3->password
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%s",buf1,buf2,buf3);
		memset(str,0,sizeof(str));
		sprintf(str,"%s%s%s%s%s","select * from user where id = '",buf2,"' and password = '",buf3,"';");
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		//sucess
		if(row)
		{
			write(confd,"0",1);
			MyFd.clifd[i].second = buf2;
			memset(str,0,sizeof(str));
			sprintf(str,"%s%s%s","update user set online = 1 where id = '",row[0],"';");
			printf("%s\n",str);
			mysql_query(&mysql,str);
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == 3)
	{
		//buf2->id
		sscanf(buf,"%[^|]%*[|]%s",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"%s%s%s","select * from user where id = '",buf2,"';");
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);		
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"%s%s","0|",row[2]);
			MyFd.clifd[i].second = buf2;
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == 4)
	{
		//buf2 -> answer
		sscanf(buf,"%[^|]%*[|]%s",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"%s%s%s%s%s","select * from user where id ='",MyFd.clifd[i].second.c_str(),"' and answer = '",buf2,"';");
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			write(confd,"0",1);
		
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == 5)
	{
		memset(str,0,sizeof(str));
		sprintf(str,"%s%s%s","delete from user where id ='",MyFd.clifd[i].second.c_str(),"';");	
		printf("%s\n",str);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == 10)
	{
//		buf2 ->password buf3 ->new password
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%s",buf1,buf2,buf3);
		memset(str,0,sizeof(str));
		sprintf(str,"%s%s%s%s%s","select * from user where id = '",MyFd.clifd[i].second.c_str(),"' and password = '",buf2,"';");
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"%s%s%s%s%s","update user set password = '",buf3,"' where id = '",MyFd.clifd[i].second.c_str(),"';");
			if(!mysql_query(&mysql,str))
			{
				write(confd,"0",1);
			
			}
			else
			{
				write(confd,"-1",2);
			}
		}
		else
		{
		
			write(confd,"-1",2);
		}
	}
	else if(num1 == 11)
	{
		//buf2 ->password buf3->newquestion buf4-> new answer
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]%*[|]%s",buf1,buf2,buf3,buf4);
		memset(str,0,sizeof(str));
		sprintf(str,"%s%s%s%s%s","select * from user where id = '",MyFd.clifd[i].second.c_str(),"' and password = '",buf2,"';");
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"%s%s%s%s%s%s%s","update user set question = '",buf3,"' , answer = '",buf4,"' where id ='",MyFd.clifd[i].second.c_str(),"';");
			printf("%s\n",str);
			if(!mysql_query(&mysql,str))
			{
				write(confd,"0",1);
			}
			else
			{
				write(confd,"-1",2);
			}
		}
		else
		{
		
			write(confd,"-1",2);
		}
	}
	else if(num1 == 12)
    {
        sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET name = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"1",1);
		}
    }
	else if(num1 == 13)
    {
        sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET gender = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"1",1);
		}
    }
	else if(num1 == 15)
    {
        sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET birth = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"1",1);
		}
    }
	else if(num1 == 16)
    {
        sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET sign = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"1",1);
		}
    }

	mysql_close(&mysql);

	
}
//g++ MyServer.cpp Init.cpp FD.cpp WorkData.cpp -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
