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
std::unordered_map<std::string,int>order{{"Reg",1},{"SignIn",2},{"ForgetPasswd1",3},{"ForgetPasswd2",4},{"DeleteId",5},{"ChangePasswd",10},{"ChangeQuestion",11},{"ChangeName",12},{"ChangePhoto",13},{"ChangeGender",14},{"ChangeBirth",15},{"ChangeSign",16},{"ViewName",30},{"ViewGender",32},{"ViewBirth",33},{"ViewSign",34}};

#endif
