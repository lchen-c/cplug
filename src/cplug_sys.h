#pragma once

#include <string>
#include <vector>

using namespace std;

string Cplus_Sys_GetCurrentThreadID();

void Cplus_Sys_SetSoftVersion(const std::string& sVersion);

string Cplus_Sys_GetSoftVersion();

long long Cplus_Sys_GetExeRunTime();

long long Cplus_Sys_GetSysRunTime();

string Cplus_Sys_GetMemoryInfo();

long long Cplus_Sys_GetThreadCnt();

long long Cplus_Sys_GetHandleCnt();

int Cplus_Sys_GetCurrentProcessID();