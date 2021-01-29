#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <map>

using namespace std;

class IShareMemory
{
	virtual char* getBuffer() = 0;
};

IShareMemory* Cplug_SM_Regist(const string& sMapName, unsigned int nSize);

char* Cplug_SM_GetBuffer(const string& sMapName);

