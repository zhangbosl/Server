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
#include "FD.h"
#include "Init.h"
void WorkData(int,int ,FD &);
#endif
