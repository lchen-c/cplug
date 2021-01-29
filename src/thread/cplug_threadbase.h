#ifndef THREADBASE_H
#define THREADBASE_H
#include <atomic>
#include <thread>
#include <mutex>               
#include <condition_variable> 

using namespace std;

class ThreadBase
{
	typedef void (*Fn_BeforeThread)();
	typedef void (*Fn_AfterThread)();

public:
	ThreadBase();
	virtual ~ThreadBase();
	void start();
	std::string getThreadId();
	void terminate();
	bool isTerminate();
	void join();	
	virtual void run();
	virtual void domain();
	virtual void setBeforeThread(Fn_BeforeThread before);
	virtual void setAfterThread(Fn_AfterThread after);
private:
	atomic<bool> m_isTerminate;
	thread thr;
	Fn_BeforeThread m_bFun;
	Fn_AfterThread m_aFun;

};

#endif