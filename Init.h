#ifndef __INITSOCK_H__
#define __INITSOCK_H__
#include <arpa/inet.h>
#include <cstring>
#include <stdio.h>
#define PORT 8659  //端口
#define  MyIP "192.168.209.130" //IP

struct sockaddr_in InitSock();
int InitBind(int,sockaddr_in &);

#endif
