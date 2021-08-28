#ifndef __FD_H__
#define __FD_H__
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <string>
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


#endif
