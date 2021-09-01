#include "WorkData.h"


void WorkData(int confd,int i,FD &MyFd)
{

	char buf[1024]={0};
	printf("i = %d\n",i);
	MYSQL mysql;
	MYSQL_RES *result = NULL;
	MYSQL_ROW row;
	mysql_init(&mysql);
   	if(mysql_real_connect(&mysql,"localhost","lingxi","19410479","chat",0,NULL,0)==NULL)
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
			sprintf(str,"#001|0|%s",row[0]);
			write(confd,str,strlen(str));
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
			write(confd,"#002|0|Sucess",13);
			MyFd.clifd[i].second = buf2;
			memset(str,0,sizeof(str));
			sprintf(str,"%s%s%s","update user set online = 1 where id = '",row[0],"';");
			printf("%s\n",str);
			mysql_query(&mysql,str);
		}
		else
		{
			write(confd,"#002|1|Failed",13);
		}
		mysql_free_result(result);
	}
	else if(num1 ==order["ViewMyName"])
	{
		memset(str,0,sizeof(str));
		sprintf(str,"select name from uinfor WHERE id = '%s';",MyFd.clifd[i].second.c_str());
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);
		memset(str,0,sizeof(str));
		sprintf(str,"#%03d|%s",order["ViewMyName"],row[0]);
		printf("%s\n",str);
		write(confd,str,strlen(str));
		mysql_free_result(result);
		sleep(1);
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
			sprintf(str,"#%03d|%s|%s",order["ViewName"],row[0],buf2);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Wrong id",order["ViewName"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ViewId"])
	{
		//buf2 -> name
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);	
		memset(str,0,sizeof(str));
		sprintf(str,"select id from uinfor WHERE name = '%s';",buf2);	
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		memset(str,0,sizeof(str));
		sprintf(str,"#%03d",order["ViewName"]);
		while(row = mysql_fetch_row(result))
		{
			strcat(str,"|");
			strcat(str,buf2);
			strcat(str,"|");
			strcat(str,row[0]);		
		}
		write(confd,str,sizeof(str));
		printf("%s\n",str);
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
			sprintf(str,"#%03d|0|%s",order["ViewGender"],row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Wrong id",order["ViewGender"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
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
			sprintf(str,"#%03d|0|%s",order["ViewBirth"],row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Wrong id",order["ViewBirth"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
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
			sprintf(str,"#%03d|0|%s",order["ViewSign"],row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
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
			{
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|0|Delete success",order["DeleteFriend"]);
				printf("%s\n",str);
				write(confd,str,strlen(str));	
			}
			else
			{
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|1|Delete error",order["DeleteFriend"]);
				printf("%s\n",str);
				write(confd,str,strlen(str));	
			}
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Delete error",order["DeleteFriend"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));	
		}
		mysql_free_result(result);
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
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|0|Change success",order["ChangeRemark"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));	
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Change error",order["ChangeRemark"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
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
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|1|You have been friends",order["AddFriend"]);
				printf("%s\n",str);
				write(confd,str,strlen(str));		
			}
			else
			{
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|0|Waiting for reply",order["AddFriend"]);
				printf("%s\n",str);
				write(confd,str,strlen(str));	
			}
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"insert into friend (id1,id2) values('%s','%s');",MyFd.clifd[i].second.c_str(),buf2);
			if(!mysql_query(&mysql,str))
			{
				memset(str,0,sizeof(str));
				sprintf(str,"select name from uinfor where id = '%s';",buf2);
				mysql_query(&mysql,str);
				result = mysql_store_result(&mysql);
				row = mysql_fetch_row(result);
				sprintf(str,"#%03d|%s|%s",order["AddFriend"],row[0],buf2);
				printf("%s\n",str);
				write(confd,str,strlen(str));	
			}
			else
			{
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|1|Insert error",order["AddFriend"]);
				printf("%s\n",str);
				write(confd,str,strlen(str));	
			}
		}
		mysql_free_result(result);
	}
	else if(num1 == order["ViewFriendRequest"])
	{
		memset(str,0,sizeof(str));
		sprintf(str,"select a.id1,b.name from friend a join uinfor b on a.id1 = b.id where a.id2 = '%s' and state = 0",MyFd.clifd[i].second.c_str());
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		memset(str,0,sizeof(str));
		sprintf(str,"#%03d",order["ViewFriendRequest"]);
		while(row = mysql_fetch_row(result))
		{
			strcat(str,"|");
			strcat(str,row[1]);
			strcat(str,"|");
			strcat(str,row[0]);
		}
		write(confd,str,strlen(str));
		printf("%s\n",str);
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
		mysql_free_result(result);
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
		mysql_free_result(result);
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
			{
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|1|Delete error",order["DeleteFriend"]);
				printf("%s\n",str);
				write(confd,str,strlen(str));	
			}
		}
		else
		{
			write(confd,"-1",2);
		}
		mysql_free_result(result);
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
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Change error",order["ChangeRemark"]);
			printf("%s\n",str);
			write(confd,str,strlen(str));	
		}
		mysql_free_result(result);		
	}
	else if(num1 == order["FriendList"])
	{
		memset(str,0,sizeof(str));
		sprintf(str,"select a.id2, b.name from friend a join uinfor b on a.id2 = b.id where a.id1 = '%s' and a.state = 1 order by a.id2 asc;",MyFd.clifd[i].second.c_str());
		printf("%s\n",str);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		memset(str,0,sizeof(str));
		sprintf(str,"#%03d",order["FriendList"]);
		while(row = mysql_fetch_row(result))
		{
			strcat(str,"|");
			strcat(str,row[1]);
			strcat(str,"|");
			strcat(str,row[0]);
		}
		write(confd,str,strlen(str));
		printf("%s\n",str);
		mysql_free_result(result);	
	}
	else if(num1 == order["SendMessage"])
	{
		//buf2-> message buf3->id
		sscanf(buf,"%[^|]%*[|]%[^|]%*[|]%[^|]",buf1,buf2,buf3);	
		memset(str,0,sizeof(str));
		const char * htime = GetTime().c_str();
		sprintf(str,"insert into history (id1,id2,time,message) values('%s','%s','%s','%s');",MyFd.clifd[i].second.c_str(),buf3,htime,buf2);
		printf("%s\n",str);
		mysql_query(&mysql,str);
		//clicd find
		int j,csize = MyFd.clifd.size();
		for(j=0;j<csize;++j)
		{
			if(strcmp(MyFd.clifd[j].second.c_str(),buf3)==0)
			{
				memset(str,0,sizeof(str));
				sprintf(str,"#%03d|%s\n%s|%s",order["SendMessage"],GetTime().c_str(),buf2,MyFd.clifd[i].second.c_str());
				printf("%s\n",str);
				write(MyFd.clifd[j].first,str,strlen(str));
				memset(str,0,sizeof(str));
				sprintf(str,"update history set state = 1 where id1 = '%s' and id2 = '%s' and time = '%s';",MyFd.clifd[i].second.c_str(),buf3,htime);
				printf("%s\n",str);
				mysql_query(&mysql,str);
				break;
			}
		}
		//generate str
	}
	else if(num1 == order["ReceMessage"])
	{
		//buf2->id
		sscanf(buf,"%[^|]%*[|]%[^|]",buf1,buf2);
		memset(str,0,sizeof(str));
		sprintf(str,"select message,time from history where id2 = '%s' and state = 0 and id1 = '%s';",MyFd.clifd[i].second.c_str(),buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		while(row = mysql_fetch_row(result))
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|%s|%s\n%s",order["ReceMessage"],buf2,row[1],row[0]);
			printf("%s\n",str);
			write(confd,str,strlen(str));
			memset(str,0,sizeof(str));
			sprintf(str,"update history set state =1 where id2 ='%s' and time = '%s';",MyFd.clifd[i].second.c_str(),row[1]);
			printf("%s\n",str);
			mysql_query(&mysql,str);
		}
		mysql_free_result(result);
	}
	else if(num1==order["FriendPhoto"])
	{
		sscanf(buf,"%[^|]%*[|]%[^|]\n",buf1,buf2);	
		memset(str,0,sizeof(str));
		sprintf(str,"select photo from uinfor where id='%s'",buf2);
		mysql_query(&mysql,str);
		result = mysql_store_result(&mysql);
		long unsigned int* lengths = mysql_fetch_lengths(result);
		
		sprintf(str,"#090|0|%ld",lengths[0]);
		write(confd,str,strlen(str));//+7+sizeof(long unsigned int)
		
		int i,packet=1024;
		for(i=0;i+packet+1<lengths[0];i+=packet)
		{
			memcpy(str,"#090|1|", 7);
			memcpy(str+7,row[0]+i, packet);
			write(confd,str,packet+7);//+7+sizeof(long unsigned int)
		}
		memcpy(str,"#090|1|", 7);
		memcpy(str+7,row[0]+i, lengths[0]-i+1);
		memcpy(str+lengths[0]-i+8,"@@*##", 5);
		write(confd,str,lengths[0]-i+13);//+7+sizeof(long unsigned int)
		mysql_free_result(result);	
	}
	else if(num1==order["SendFile"])
	{
		//读文件，存文件
		char lstr[10*1024]={0};
		mysql_query(&mysql, "SELECT image,id FROM exm order by id desc limit 1");
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);//printf("%s\n",row[1]);
		long unsigned int* lengths = mysql_fetch_lengths(result);
		//std::cout<<lengths[0]<<row[0]<<std::endl;
		
		sprintf(lstr,"#099|0|%ld",lengths[0]);
		write(confd,lstr,strlen(lstr));//+7+sizeof(long unsigned int)
		printf("%s\n",lstr);
		sleep(1);
		int i,packet=1024;
		for(i=0;i+packet+1<lengths[0];i+=packet)
		{
			//memset(str,0,sizeof(str));
			memcpy(lstr,"#099|1|", 7);
			memcpy(lstr+7,row[0]+i, packet);
			//memcpy(lstr+1031,"@@*##", 5);
			//printf("%s\n",str);
			write(confd,lstr,packet+7);//+7+sizeof(long unsigned int)
			sleep(0.8);
		}
		printf("ok\n");
		memset(lstr,0,sizeof(lstr));
		memcpy(lstr,"#099|1|", 7);
		memcpy(lstr+7,row[0]+i, lengths[0]-i+1);
		memcpy(lstr+lengths[0]-i+8,"@@*##", 5);
		write(confd,lstr,lengths[0]-i+13);//+7+sizeof(long unsigned int)
		mysql_free_result(result);	
		//write(confd,b_str,strlen(b_str));
		//write(confd,row[0],lengths[0]);//+7+sizeof(long unsigned int)
	}
	else if(num1==order["RecvFile"])
	{		
		char lstr[10*1024]={0};
		mysql_query(&mysql, "SELECT image,id FROM exm order by id desc limit 1");
		result = mysql_store_result(&mysql);
		row = mysql_fetch_row(result);//printf("%s\n",row[1]);
		long unsigned int* lengths = mysql_fetch_lengths(result);
		//std::cout<<lengths[0]<<row[0]<<std::endl;
		
		sprintf(lstr,"#099|0|%ld",lengths[0]);
		write(confd,lstr,strlen(lstr));//+7+sizeof(long unsigned int)
		printf("%s\n",lstr);
		sleep(1);
		int i,packet=1024;
		for(i=0;i+packet+1<lengths[0];i+=packet)
		{
			//memset(str,0,sizeof(str));
			memcpy(lstr,"#099|1|", 7);
			memcpy(lstr+7,row[0]+i, packet);
			//memcpy(lstr+1031,"@@*##", 5);
			//printf("%s\n",str);
			write(confd,lstr,packet+7);//+7+sizeof(long unsigned int)
			sleep(0.8);
		}
		printf("ok\n");
		memset(lstr,0,sizeof(lstr));
		memcpy(lstr,"#099|1|", 7);
		memcpy(lstr+7,row[0]+i, lengths[0]-i+1);
		memcpy(lstr+lengths[0]-i+8,"@@*##", 5);
		write(confd,lstr,lengths[0]-i+13);//+7+sizeof(long unsigned int)
		mysql_free_result(result);	
		//write(confd,b_str,strlen(b_str));
		//write(confd,row[0],lengths[0]);//+7+sizeof(long unsigned int)
	}
	mysql_close(&mysql);
}
std::string GetTime()
{
	time_t t;
	t=time(NULL);
	struct tm *m_time;
	m_time=localtime(&t);
	char szDateTime[100] = { 0 };
	sprintf(szDateTime, "%04d-%02d-%02d %02d:%02d:%02d", 1900+m_time->tm_year, m_time->tm_mon,m_time->tm_mday, m_time->tm_hour, m_time->tm_min,m_time->tm_sec);
	printf("time is %s\n",szDateTime);
	return szDateTime;
}
//g++ MyServer.cpp Init.cpp FD.cpp WorkData.cpp -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
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
			sprintf(str,"#%03d|0|%s",order["ForgetPasswd1"],row[2]);
			MyFd.clifd[i].second = buf2;
			write(confd,str,strlen(str));
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Wrong account",order["ForgetPasswd1"]);
			write(confd,str,strlen(str));
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
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|0",order["ForgetPasswd"]);
			write(confd,str,strlen(str));
		}
		else
		{
			memset(str,0,sizeof(str));
			sprintf(str,"#%03d|1|Wrong Answer",order["ForgetPasswd"]);
			write(confd,str,strlen(str));
		}
		mysql_free_result(result);
	}*/
