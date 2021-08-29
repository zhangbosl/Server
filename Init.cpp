#include "Init.h"
struct sockaddr_in InitSock()
{
	//实现连接
	//结构体

	struct sockaddr_in sevaddr;  
	memset(&sevaddr,0,sizeof(sevaddr));  //置零
	//初始化
	sevaddr.sin_family = AF_INET;  //IPv4
	sevaddr.sin_port = htons(PORT);  //端口
	sevaddr.sin_addr.s_addr =inet_addr(MyIP) ;  //IP
	return sevaddr;	
}

int InitBind(int lisfd,sockaddr_in &sevaddr)
{
	if(bind(lisfd, (struct sockaddr *)&sevaddr,sizeof(sevaddr)))
	{
		printf("bind error\n");
		return -1;
	}
	return 0;
}
