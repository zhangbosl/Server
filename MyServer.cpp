#include "MyServer.h"
int main()
{
	//结构体初始化 
	struct sockaddr_in sevaddr=InitSock();
	//类FD 
	FD MyFd;

	int optval = 1;
	//socket
	MyFd.lisfd = socket(AF_INET,SOCK_STREAM,0);//数据报 DGRAM 数据流 STREAM
	setsockopt(MyFd.lisfd,SOL_SOCKET,SO_REUSEADDR,&optval,sizeof(optval));
	//bind 绑定
	if(InitBind(MyFd.lisfd,sevaddr))
	{
		return -1;
	}
	
	listen(MyFd.lisfd,10);
	

	fd_set reset;
	int confd;
	//allset清空
	MyFd.zero();

	//把lisfd放入allset
	MyFd.set(MyFd.lisfd);
	
	while(1)
	{
		reset = MyFd.allset;
		//select
		int nready = select(MyFd.maxfd+1,&reset,NULL,NULL,NULL);
		if(nready <= 0)
		{
			printf("select error\n");
			return -1;	
		}

		//如果lisfd可以接通 则accept
		if(FD_ISSET(MyFd.lisfd,&reset))
		{
			confd = accept(MyFd.lisfd,NULL,NULL);
			if( confd == -1)
			{
				printf("accept error\n");
				return -1;
			}

			//把confd 放入clifd和allset
			MyFd.clifd.push_back(std::pair<int,std::string>(confd,""));
			MyFd.set(confd);		
		}
		
		for(int i=0;i<MyFd.clifd.size();i++)
		{
			confd = MyFd.clifd[i].first;
			//如果 confd可以接通 处理数据
			if(FD_ISSET(confd,&reset))
			{
				//处理数据

				WorkData(confd,i,MyFd);
			}		
		}
	}
	return 0;
}
//g++ MyServer.o Init.o FD.o WorkData.o -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient
//g++ MyServer.cpp Init.cpp FD.cpp WorkData.cpp -L/usr/lib/x86_64-linux-gnu/mysql -lmysqlclient

