#include "WorkData.h"


void WorkData(int confd,int i,FD &MyFd)
{
	
	char buf[1024]={0};
	printf("i = %d\n",i);
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row;
	mysql_init(&mysql);
   	if(mysql_real_connect(&mysql,"localhost","root","","chat",0,NULL,0)==NULL)
   	{
		printf("connect error: %s\n",mysql_error(&mysql));
		return ;
   	}
   	else
		printf("connected\n");
	char buf1[1024]={0},buf2[1024]={0},buf3[1024]={0},buf4[1024]={0},buf5[1024]={0},str[1024]={0},sqlStr[1024]={0};
	//断开连接
	if(!read(confd,buf,sizeof(buf)))
	{
		memset(str,0,sizeof(str));
		sprintf(str,"update user set online = 0 where id ='%s';",MyFd.clifd[i].second.c_str());
		printf("%s\n",str);
		mysql_query(&mysql,str);
		//allset和clifd中删除
		FD_CLR(confd,&MyFd.allset);
		MyFd.clifd.erase(MyFd.clifd.begin()+i);

		return ;
	}
	//如果读入了数据在buf中
	printf("buf=%s\n",buf);
	
	
	sscanf(buf,"%[^|]",buf1);
	if(buf1[0]=='#')
		sscanf(buf1,"#%s",buf1);
	int num1=atoi(buf1);printf("num = %s\n",buf1);
	
	
	
	if(num1 == order["Reg"])
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
		
		if(row)//&&row[2]==buf3&&row[3]==buf4&&row[4]==buf5
		{
			write(confd,"#001|0|regSucess",16);
			MyFd.clifd[i].second=row[0];
			
			sprintf(sqlStr,"insert into uinfor (id,name) values('%s','%s')",row[0],buf2);
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
	else if(num1 == order["SignIn"])
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
			write(confd,"#002|0|",7);
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
	else if(num1 == order["ForgetPasswd1"])
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
	else if(num1 == order["ForgetPasswd2"])
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
	else if(num1 == order["DeleteId"])
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
	else if(num1 == order["ChangePasswd"])
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
		mysql_free_result(result);
	}
	else if(num1 == order["ChangeQuestion"])
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
		mysql_free_result(result);
	}
	else if(num1 == order["ChangeName"])
	{
		// buf2 -> new name
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET name = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,sqlStr))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ChangeGender"])
	{
		//buf2 -> new gender
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET gender = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,sqlStr))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ChangeBirth"])
	{
		//buf2 -> new birth
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET birth = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,sqlStr))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ChangeSign"])
	{
		//buf2 -> new sign
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		sprintf(sqlStr,"UPDATE uinfor SET sign = '%s' WHERE id = '%s';",buf2,MyFd.clifd[i].second.c_str());	
		printf("%s\n",sqlStr);
		if(!mysql_query(&mysql,sqlStr))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	
	else if(num1 == order["ViewName"])
	{
		//buf2 -> ID
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select name from uinfor WHERE id = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ViewGender"])
	{
		//buf2 -> ID
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select Gender from uinfor WHERE id = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ViewBirth"])
	{
		//buf2 -> ID
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select birth from uinfor WHERE id = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ViewSign"])
	{
		//buf2 -> ID
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select sign from uinfor WHERE id = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 == order["AddFriend"])
	{
		//buf2 -> ID
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);	
		memset(str,0,sizeof(str));
		sprintf(str,"select state from friend WHERE id1 = '%s' and id2 = '%s';",buf2,MyFd.clifd[i].second.c_str());
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{	if(atoi(row[0]) == 1)
			{
				printf("You have been friends\n");
				write(confd,"-1",2);			
			}
			else
			{
				write(confd,"0",1);
			}
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"insert into friend (id1,id2) values('%s','%s');",MyFd.clifd[i].second.c_str(),buf2);
			if(!mysql_query(&mysql,str))
			{
				write(confd,"0",1);
			}
			else
			{
				write(confd,"-1",2);
			}
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ViewFriendRequest"])
	{
		memset(str,0,sizeof(str));
		sprintf(str,"select id1 from friend where id2 = '%s' and state = 0",MyFd.clifd[i].second.c_str());
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		memset(str,0,sizeof(str));
		sprintf(str,"0");
		while(row = mysql_fetch_row(result))
		{
			strcat(str,"|");
			strcat(str,row[0]);
		}
		write(confd,str,strlen(str));
		mysql_free_result(result);
	}
	else if(num1 == order["AgreeRequest"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);	
		memset(str,0,sizeof(str));
		sprintf(str,"select * from friend WHERE id1 = '%s' and id2 = '%s' and state = 0;",buf2,MyFd.clifd[i].second.c_str());
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"update friend set state = 1 WHERE id1 = '%s' and id2 = '%s' and state = 0;",buf2,MyFd.clifd[i].second.c_str());
			if(!mysql_query(&mysql,str))
			{
				memset(str,0,sizeof(str));
				sprintf(str,"insert into friend (id1,id2,state) values('%s','%s',1);",MyFd.clifd[i].second.c_str(),buf2);
				printf("%s\n");
				if(!mysql_query(&mysql,str))
					write(confd,"0",1);
				else
					write(confd,"-1",2);
			}
			else
			{
				write(confd,"-1",2);
			}
		}
	
	
	}
	else if(num1 == order["RejectRequest"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);	
		memset(str,0,sizeof(str));
		sprintf(str,"delete from friend WHERE id1 = '%s' and id2 = '%s' and state = 0;",buf2,MyFd.clifd[i].second.c_str());
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}
	}
	else if(num1 == order["DeleteFriend"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);	
		memset(str,0,sizeof(str));
		sprintf(str,"delete from friend WHERE id1 = '%s' and id2 = '%s';",buf2,MyFd.clifd[i].second.c_str());
		if(!mysql_query(&mysql,str))
		{
			memset(str,0,sizeof(str));
			sprintf(str,"delete from friend WHERE id1 = '%s' and id2 = '%s';",MyFd.clifd[i].second.c_str(),buf2);
			if(!mysql_query(&mysql,str))
				write(confd,"0",1);
			else
				write(confd,"-1",2);
		}
		else
		{
			write(confd,"-1",2);
		}
	}
	
	else if(num1 == order["ChangeRemark"])
	{
		//buf2 -> ID
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]\n",buf1,buf2,buf3);	
		memset(str,0,sizeof(str));
		sprintf(str,"update friend set remark = '%s' where id1 = '%s' and id2= '%s';",buf3,MyFd.clifd[i].second.c_str(),buf2);
		mysql_query(&mysql,str);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}		
	}
	
	
	else if(num1 == order["FriendList"])
	{
		memset(str,0,sizeof(str));
		sprintf(str,"select id2 from friend where id1 = '%s' and state = 1",MyFd.clifd[i].second.c_str());
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		memset(str,0,sizeof(str));
		sprintf(str,"0");
		while(row = mysql_fetch_row(result))
		{
			strcat(str,"|");
			strcat(str,row[0]);
		}
		write(confd,str,strlen(str));
		mysql_free_result(result);	
	
	}
	/*
	else if(num1 == order["AddGroup"])
	{
	
	
	}
	else if(num1 ==order["ViewGName"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select name from ginfor WHERE gid = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
		
	}
	else if(num1 ==order["ViewGLeader"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select uid from ginfor WHERE gid = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 ==order["ViewGIntro"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select intro from ginfor WHERE gid = '%s';",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		if(row)
		{
			memset(str,0,sizeof(str));
			sprintf(str,"0|%s",row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
	}
	else if(num1 ==order["ChangeGName"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]\n",buf1,buf2,buf3);	
		memset(str,0,sizeof(str));
		sprintf(str,"update ginfor set name = '%s' where gid = '%s' and uid= '%s';",buf3,buf2,MyFd.clifd[i].second.c_str());
		mysql_query(&mysql,str);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}	
	}
	else if(num1 ==order["ChangeGIntro"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]\n",buf1,buf2,buf3);	
		memset(str,0,sizeof(str));
		sprintf(str,"update ginfor set intro = '%s' where gid = '%s'and uid= '%s';",buf3,buf2,MyFd.clifd[i].second.c_str());
		mysql_query(&mysql,str);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}	
	}
	else if(num1 ==order["ChangeGRemark"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]\n",buf1,buf2,buf3);	
		memset(str,0,sizeof(str));
		sprintf(str,"update gmember set remark = '%s' where gid = '%s' and uid= '%s';",buf3,buf2,MyFd.clifd[i].second.c_str());
		mysql_query(&mysql,str);
		if(!mysql_query(&mysql,str))
		{
			write(confd,"0",1);
		}
		else
		{
			write(confd,"-1",2);
		}	
	}
	
	*/
	mysql_close(&mysql);

}
//g++ MyServer.cpp Init.cpp FD.cpp WorkData.cpp -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
