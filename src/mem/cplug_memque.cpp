#include "cplug_memque.h"
#include "memoryqueue.h"
#include "../cplug_singleton.h"

IMemQue* Cplug_MQ_Regist(const string& sQueName, unsigned int nSize, MEMORY_TYPE type)
{
	TunnelManger& tunnel = Cplug_Singleton<TunnelManger>::getInstance();
	return tunnel.Regist(sQueName, nSize, type);
}

bool Cplug_MQ_PushMsg(const string& sQueName, const char* pMsg, unsigned int nSize)
{
	TunnelManger& tunnel = Cplug_Singleton<TunnelManger>::getInstance();
	return tunnel.pushMsg(sQueName, pMsg, nSize);
}

bool Cplug_MQ_PopMsg(const string& sQueName, vector<char>& vec)
{
	TunnelManger& tunnel = Cplug_Singleton<TunnelManger>::getInstance();
	return tunnel.popMsg(sQueName, vec);
}

IMemQue* Cplug_MQ_GetMemQue(const string& sQueName)
{
	TunnelManger& tunnel = Cplug_Singleton<TunnelManger>::getInstance();
	return tunnel.getMemQue(sQueName);
}
