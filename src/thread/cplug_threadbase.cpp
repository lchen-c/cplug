#include "cplug_threadbase.h"
#include <functional>
#include <sstream>
#include "../cplug_sys.h"

ThreadBase::ThreadBase()
{
	m_bFun = nullptr;
	m_aFun = nullptr;
}  

ThreadBase::~ThreadBase()
{	
	if (!this->isTerminate())
	{		
		this->terminate();
	} 	
	
	if (this->thr.joinable())
	{		
		this->thr.join();
	}
} 

void ThreadBase::start()
{	
	this->m_isTerminate = false;
	thread thr(std::bind(&ThreadBase::run,this));	
	this->thr = std::move(thr);
	
} 

std::string ThreadBase::getThreadId()
{	
	return Cplus_Sys_GetCurrentThreadID();
}

void ThreadBase::terminate()
{	
	this->m_isTerminate = true;
} 

bool ThreadBase::isTerminate()
{	
	return this->m_isTerminate;
} 

void ThreadBase::join()
{	
	this->thr.join();
} 

void ThreadBase::run()
{ 

	if (m_bFun)
		(*m_bFun)();

	domain();

	if (m_aFun)
		(*m_aFun)();
}

void ThreadBase::domain()
{
}

void ThreadBase::setBeforeThread(Fn_BeforeThread before)
{
	m_bFun = before;
}

void ThreadBase::setAfterThread(Fn_AfterThread after)
{
	m_aFun = after;
}

