#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <map>
#include "cplug_memque.h"
using namespace std;

typedef struct _T_MESSAGE_HEAD
{
	unsigned int nMsgHeadSize;
}T_MESSAGE_HEAD;

class MemoryQueue : public IMemQue
{
public:
	MemoryQueue(string sQueueName, char* pMemory, unsigned int nSize, bool bAutoDel = true);
	~MemoryQueue();
	bool pushMsg(const char* pMsg, unsigned int nSize);
	bool popMsg(vector<char>& vec);
	bool isEmpty();
	void printInfo(T_SHARED_MEMORY_HEAD* pMemoryHead);
	void setReadIndex(unsigned int index);
	void setWriteIndex(unsigned int index);
private:
	unsigned int __getLeftSize(T_SHARED_MEMORY_HEAD* pMemoryHead);
	void __writeMemory(T_SHARED_MEMORY_HEAD* pMemoryHead, const char* pMsg, unsigned int nSize);
	void __readMemory(T_SHARED_MEMORY_HEAD* pMemoryHead, char* pMsg, unsigned int nSize);
	string m_sQueueName;
	int m_nQueueSize;
	char* m_pMemory;
	bool m_bAutoDelete;
	mutex m_lock;
};

class TunnelManger
{
public:
	TunnelManger();
	~TunnelManger();
	MemoryQueue* Regist(const string& sQueName, unsigned int nSize, MEMORY_TYPE type = NORMAL_MEMORY);
	bool pushMsg(const string& sQueName, const char* pMsg, unsigned int nSize);
	bool popMsg(const string& sQueName, vector<char>& vec);
	MemoryQueue* getMemQue(const string& sQueName);
private:
	map<string, MemoryQueue*> m_mpTunnels;
};

