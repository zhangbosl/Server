#ifndef __INITSOCK_H__
#define __INITSOCK_H__
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#define PORT 8659  //端口
#define  MyIP "192.168.43.26 " //IP192.168.209.130 NAT
//10.172.200.15 school local
//192.168.43.26 
struct sockaddr_in InitSock();
int InitBind(int,sockaddr_in &);

#endif
