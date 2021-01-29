#include "cplug_sharemem.h"
#include "../cplug_singleton.h"
#include "sharememory.h"
IShareMemory* Cplug_SM_Regist(const string& sMapName, unsigned int nSize)
{
	ShareMemoryPool& pool = Cplug_Singleton<ShareMemoryPool>::getInstance();
	return pool.RegistShareMemory(sMapName, nSize);
}

char* Cplug_SM_GetBuffer(const string& sMapName)
{
	ShareMemoryPool& pool = Cplug_Singleton<ShareMemoryPool>::getInstance();
	return pool.getBuffer(sMapName);
}