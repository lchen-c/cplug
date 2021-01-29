#pragma once
#include <string>
#include <vector>
#include <mutex>
#include <map>

using namespace std;

typedef struct _T_SHARED_MEMORY_HEAD
{
	unsigned int nHeadSize;
	unsigned int nBodySize;
	unsigned int nReadIndex;
	unsigned int nWriteIndex;
	unsigned int nLeftSize;
	bool bIsEmpty;
}T_SHARED_MEMORY_HEAD;

class IMemQue
{
public:
	virtual bool pushMsg(const char* pMsg, unsigned int nSize) = 0;
	virtual bool popMsg(vector<char>& vec) = 0;
	virtual bool isEmpty() = 0;
	virtual void printInfo(T_SHARED_MEMORY_HEAD* pMemoryHead) = 0;
	virtual void setReadIndex(unsigned int index) = 0;
	virtual void setWriteIndex(unsigned int index) = 0;
};

enum MEMORY_TYPE { NORMAL_MEMORY = 0, SHARED_MEMORY };

IMemQue* Cplug_MQ_Regist(const string& sQueName, unsigned int nSize, MEMORY_TYPE type = NORMAL_MEMORY);

bool Cplug_MQ_PushMsg(const string& sQueName, const char* pMsg, unsigned int nSize);

bool Cplug_MQ_PopMsg(const string& sQueName, vector<char>& vec);

IMemQue* Cplug_MQ_GetMemQue(const string& sQueName);
