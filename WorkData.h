#ifndef __WORKDATA_H__
#define __WORKDATA_H__
#include <vector>
#include <sys/types.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <unistd.h>
class FD
{
public:
	std::vector<int>clifd;
	int maxfd = 0;
	int lisfd;
	fd_set allset;
	void set(int);
	void zero();
};
void WorkData(int,int ,FD &);


#endif
