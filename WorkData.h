#ifndef __WORKDATA_H__
#define __WORKDATA_H__
#include <vector>
#include <sys/types.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <string>
#include "FD.h"

void WorkData(int,int ,FD &);
std::unordered_map<std::string,int>order{{"reg",1},{"SignIn",2},{"ForgetPasswd1",3},{"ForgetPasswd2",4}};

#endif
