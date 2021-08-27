#ifndef __MYSERVER_H__
#define __MYSERVER_H__

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
struct sockaddr_in InitSock();
int InitBind(int,sockaddr_in &);

class FD
{
public:
	std::vector<std::pair<int,std::string>>clifd;
	int maxfd = 0; // allset中最大的fd
	int lisfd;
	fd_set allset;
	void set(int);   //把fd放入allset
	void zero();   //清空
};
void WorkData(int,int ,FD &);
#endif
