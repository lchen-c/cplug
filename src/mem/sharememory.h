#pragma once
#include <map>
#include <string>
#if defined (_WIN32) || defined(_WIN64)
#include <windows.h>
#elif defined(_unix_) || defined(_linux_)

#endif	
#include "cplug_sharemem.h"
#include <boost/unordered_map.hpp>

using namespace std;


class ShareMemory : public IShareMemory
{
public:
	ShareMemory(const string& sMapName, unsigned int nSize);
	~ShareMemory();
	virtual char* getBuffer();
private:
#if defined (_WIN32) || defined(_WIN64)
	HANDLE m_hFileMap;
#elif defined(_unix_) || defined(_linux_)
#endif	
	string m_sMapName;
	char* m_pBuffer;
};

class ShareMemoryPool
{
public:
	ShareMemory* RegistShareMemory(const string& sMapName, unsigned int nSize);
	char* getBuffer(string sMapName);
	boost::unordered_map<string, ShareMemory*> m_mpShareMem;

private:
};