#include "FD.h"

void FD::set(int fd)
{
	FD_SET(fd,&allset);
	if(fd > maxfd)
	{
		maxfd = fd;
	}
}
void FD::zero()
{
	FD_ZERO(&allset);
}
