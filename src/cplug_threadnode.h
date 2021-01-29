#pragma once
#include "mutex/cplug_semaphore.h"
#include <string>
#include "thread/cplug_threadpool.h"
#include <vector>
#include "thread/cplug_threadbase.h"
#include <map>
#include <atomic>
#include "mem/cplug_memque.h"
#include <list>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
using namespace std;

class IThreadNode : public ThreadBase
{
public:
	IThreadNode(string NodeType, int index);
	virtual ~IThreadNode();
	virtual void init(IMemQue* que1, IMemQue* que2);
	virtual void registNextNode(IThreadNode* pNode);
	virtual void notify();
	virtual void run();
	virtual int getIndex();

protected:
	virtual int getTask();
	virtual void preProcess();
	virtual void postProcess();
	virtual void notifyThread();
	string getNodeName();
	string m_NodeName;
	std::threadpool* m_pThreadPools;
	vector<ThreadBase*> m_threads;
	int m_TaskCnt;
	int m_ThreadCnt;
	map<string, IThreadNode*> m_mpThreadNode;
	IMemQue* m_MemQue1;
	IMemQue* m_MemQue2;
	list<vector<char>> m_list;
	int m_index;
};

class SemaphoreMgr
{
public:
	~SemaphoreMgr();
	void registSemaphore(const string& NodeType);
	Semaphore* getSemapore(const string& NodeType);
private:
	map<string, Semaphore*> m_mpSemaphore;
};