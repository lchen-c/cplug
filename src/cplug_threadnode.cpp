#include "cplug_threadnode.h"
#include "log/cplug_log.h"
#include "cplug_singleton.h"

IThreadNode::IThreadNode(string NodeType, int index) : m_NodeName(NodeType), m_index(index)
{
	Cplug_Singleton<SemaphoreMgr>::getInstance().registSemaphore(NodeType);
	m_MemQue1 = nullptr;
	m_MemQue2 = nullptr;
}

IThreadNode::~IThreadNode()
{

}

void IThreadNode::init(IMemQue* que1, IMemQue* que2)
{
	m_MemQue1 = que1;
	m_MemQue2 = que2;
	if (!m_MemQue1)
		logWarn("[%s] has null m_MemQue1", m_NodeName.c_str());
	if (!m_MemQue2)
		logWarn("[%s] has null m_MemQue2", m_NodeName.c_str());
}

void IThreadNode::registNextNode(IThreadNode* pNode)
{
	if (pNode)
	{
		string sNodeName = pNode->getNodeName() + "_" + to_string(pNode->getIndex());
		if (m_mpThreadNode.find(sNodeName) == m_mpThreadNode.end())
		{
			m_mpThreadNode[sNodeName] = pNode;
		}
		else
		{
			logError("[%s] has exits, regist next node failed ", sNodeName.c_str());
		}
	}
}

void IThreadNode::run()
{
	if (!m_MemQue1)
		logWarn("[%s] has null m_MemQue1", m_NodeName.c_str());
	if (!m_MemQue2)
		logWarn("[%s] has null m_MemQue2", m_NodeName.c_str());
	if (m_mpThreadNode.empty())
		logWarn("[%s] has empty threadNode", m_NodeName.c_str());

	while (!isTerminate())
	{
		int m_TaskCnt = 0;
		Cplug_Singleton<SemaphoreMgr>::getInstance().getSemapore(m_NodeName)->wait();
		m_TaskCnt = getTask();
		
		if (m_TaskCnt > 0)
		{
			preProcess();
			notifyThread();
			postProcess();
		}
	}
}

int IThreadNode::getIndex()
{
	return m_index;
}


int IThreadNode::getTask()
{
	if (!m_MemQue1)
	{
		logError("[%s] m_MemQue1 is null", m_NodeName.c_str());
		return 0;
	}
	m_list.clear();
	vector<char> vData;
	int i = 0;
	while (true)
	{
		if (m_MemQue1->popMsg(vData))
		{
			string task = string(vData.data(), vData.size());
			printf("%s get Msg: %s\n", m_NodeName.c_str(), task.c_str());
			m_list.push_back(vData);
		}
		else
		{
			break;
		}
	}
	return m_list.size();
}

void IThreadNode::preProcess()
{
}

void IThreadNode::postProcess()
{
}

void IThreadNode::notifyThread()
{
	for (auto it = m_mpThreadNode.begin(); it != m_mpThreadNode.end(); it++)
	{
		IThreadNode* pNode = (it->second);
		pNode->notify();
	}
}

string IThreadNode::getNodeName()
{
	return m_NodeName;
}

void IThreadNode::notify()
{
	Cplug_Singleton<SemaphoreMgr>::getInstance().getSemapore(m_NodeName)->signal();
}

SemaphoreMgr::~SemaphoreMgr()
{
	for (auto it = m_mpSemaphore.begin(); it != m_mpSemaphore.end(); )
	{
		if (nullptr != it->second)
		{
			delete it->second;
			m_mpSemaphore.erase(it++);
		}
	}
}

void SemaphoreMgr::registSemaphore(const string& NodeType)
{
	if (m_mpSemaphore.find(NodeType) == m_mpSemaphore.end())
	{
		m_mpSemaphore[NodeType] = new Semaphore(0, 100);
	}
}

Semaphore* SemaphoreMgr::getSemapore(const string& NodeType)
{
	if (m_mpSemaphore.find(NodeType) != m_mpSemaphore.end())
		return m_mpSemaphore[NodeType];
	return nullptr;
}
