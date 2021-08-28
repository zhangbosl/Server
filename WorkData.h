#ifndef __WORKDATA_H__
#define __WORKDATA_H__
#include <vector>
#include <sys/types.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <cstring>
#include <unistd.h>
#include <string>
class FD
{
public:
	std::vector<std::pair<int,std::string>>clifd;
	int maxfd = 0;
	int lisfd;
	fd_set allset;
	void set(int);
	void zero();
};
void WorkData(int,int ,FD &);


#endif
