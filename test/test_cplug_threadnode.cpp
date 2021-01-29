#include "catch.hpp"
#include <cplug_threadnode.h>
#include <log/cplug_log.h>
#include <thread>


class TestNormal1 : public IThreadNode
{
public:
	TestNormal1(string NodeType, int index) : IThreadNode(NodeType, index) {}
protected:
	virtual void preProcess() 
	{
		for (auto it = m_list.begin(); it != m_list.end(); it++)
		{
			string sJson = string(it->data(), it->size());
			logInfo("AAA[%d] preProcess, num[%s]", m_index, sJson.c_str());
			std::this_thread::sleep_for(chrono::milliseconds(10));
			m_MemQue2->pushMsg(sJson.data(), sJson.length());
		}
	}
	virtual void postProcess() { logInfo("AAA postProcess"); }
};

class TestNormal2 : public IThreadNode
{
public:
	TestNormal2(string NodeType, int index) : IThreadNode(NodeType, index) {}
protected:
	virtual void preProcess()
	{
		for (auto it = m_list.begin(); it != m_list.end(); it++)
		{
			string sJson = string(it->data(), it->size());
			logInfo("BBB[%d] preProcess, num[%s]", m_index, sJson.c_str());
			std::this_thread::sleep_for(chrono::milliseconds(10));
			m_MemQue2->pushMsg(sJson.data(), sJson.length());
		}
	}
	virtual void postProcess() { logInfo("BBB postProcess"); }
};

class TestNormal3 : public IThreadNode
{
public:
	TestNormal3(string NodeType, int index) : IThreadNode(NodeType, index) {}
protected:
	virtual void preProcess()
	{
		for (auto it = m_list.begin(); it != m_list.end(); it++)
		{
			string sJson = string(it->data(), it->size());
			logInfo("CCC[%d] preProcess, num[%s]", m_index, sJson.c_str());
			std::this_thread::sleep_for(chrono::milliseconds(10));

		}
	}
	virtual void postProcess() { logInfo("CCC postProcess"); }
};

TEST_CASE("1:Test threadnode", "[ThreadNode]")
{
	IMemQue* mq_aaa = Cplug_MQ_Regist("QUE_AAA", 1024 * 1024);
	IMemQue* mq_bbb = Cplug_MQ_Regist("QUE_BBB", 1024 * 1024);
	IMemQue* mq_ccc = Cplug_MQ_Regist("QUE_CCC", 1024 * 1024);

	///////////////////////////////////////////////////////

	TestNormal3* p3_1 = new TestNormal3("THREAD_CCC", 1);
	p3_1->init(mq_ccc, NULL);

	TestNormal3* p3_2 = new TestNormal3("THREAD_CCC", 2);
	p3_2->init(mq_ccc, NULL);

	TestNormal3* p3_3 = new TestNormal3("THREAD_CCC", 3);
	p3_3->init(mq_ccc, NULL);

	///////////////////////////////////////////////////////

	TestNormal2* p2_1 = new TestNormal2("THREAD_BBB", 1);
	p2_1->init(mq_bbb, mq_ccc);
	p2_1->registNextNode(p3_1);

	TestNormal2* p2_2 = new TestNormal2("THREAD_BBB", 2);
	p2_2->init(mq_bbb, mq_ccc);
	p2_2->registNextNode(p3_2);

	TestNormal2* p2_3 = new TestNormal2("THREAD_BBB", 3);
	p2_3->init(mq_bbb, mq_ccc);
	p2_3->registNextNode(p3_3);

	///////////////////////////////////////////////////////

	TestNormal1* p1 = new TestNormal1("THREAD_AAA", 1);
	p1->init(mq_aaa, mq_bbb);
	p1->registNextNode(p2_1);
	p1->registNextNode(p2_2);
	p1->registNextNode(p2_3);

	p3_1->start();
	p3_2->start();
	p3_3->start();
	p2_1->start();
	p2_2->start();
	p2_3->start();
	p1->start();
	
	int i = 0;
	while (i < 2000)
	{
		i++;
		string str = to_string(i);
		Cplug_MQ_PushMsg("QUE_AAA", str.data(), str.length());
		p1->notify();
		std::this_thread::sleep_for(chrono::milliseconds(1));
	}

	std::this_thread::sleep_for(chrono::seconds(111111111));

}