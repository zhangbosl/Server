#ifndef __WORKDATA_H__
#define __WORKDATA_H__
#include <vector>
#include <sys/types.h>
#include <mysql/mysql.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <unistd.h>
#include <string>
#include <sys/time.h>
#include "FD.h"
std::string GetTime();
void WorkData(int,int ,FD &);
std::unordered_map<std::string,int>order\
{{"Reg",1},{"SignIn",2},\
{"ViewName",50},{"ViewId",51},{"ViewMyName",25},{"FriendList",34},\
{"AddFriend",30},{"ViewFriendRequest",31},{"AgreeRequest",32},{"RejectRequest",35},\
{"SendMessage",40},{"ReceMessage",41},\
{"ForgetPasswd1",3},{"ForgetPasswd2",4},{"DeleteId",5},\
{"ChangePasswd",10},{"ChangeQuestion",11},{"ChangeName",12},{"ChangePhoto",13},\
{"ChangeGender",14},{"ChangeBirth",15},{"ChangeSign",16},\
{"ViewGender",22},{"ViewBirth",23},{"ViewSign",24},\
{"DeleteFriend",36},{"ChangeRemark",11111},\
{"AddGroup",49},{"ViewGName",55},{"ViewGLeader",42},\
{"ViewGIntro",43},{"ChangeGName",44},{"ChangeGIntro",45},{"ChangeGRemark",46},{"ExitGroup",47}};

// reg signin viewName viewMyName FriendList 
//AddFriend ViewFriendRequest Aggree Reject SendMessage ReceMessage
#endif
