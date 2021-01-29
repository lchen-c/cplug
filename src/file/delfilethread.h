#ifndef DELFILETHREAD_H
#define DELFILETHREAD_H
#include "../log/cplug_log.h"
#include <map>
#include "../thread/cplug_threadbase.h"
#include <string>
#include <mutex>
#if defined (_WIN32) || defined(_WIN64)
#include <io.h>
#elif defined(_unix_) || defined(_linux_)
#include <sys/io.h>
#endif
#include <stdio.h>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

/*
调用示例：
//引用头文件
#include "DelFileThread.h"
//初始化线程
TDelFileThread::StartDelFileThread(); 
//添加要删除的目录
TDelFileThread::AddDelFileTask("C:\File","*.*",10,2); //删除C:\File下所有10天以前的文件，包含子文件夹   
//移除要删除的目录（移除后线程不再遍历此目录）
TDelFileThread::RemoveDelFileTask("C:\File","*.*"); //删除C:\File下所有10天以前的文件，包含子文件夹
//停止线程
TDelFileThread::StopDelFileThread();
*/

//---------------------------------------------------------------------------
//路径，文件类型，保留时间
struct DELFILE_T
{
    string sFilePath; //文件路径，例如 "C:\"
	string sFileType; //文件类型，例如 "sav.*"，所有文件用"*.*"
    int iDelDay;          //删除天数，超过这个时间之前的文件将会被删除，例如值为5，则5天前的文件会被删除，如果为0则不会执行删除
    int iDelOption;       //删除选项：0 - 删除根目录下的文件
                          //          1 - 删除根目录下的文件，和子文件夹内过期的文件
                          //          2 - 删除根目录下的文件，和子文件夹内过期的文件，和过期的子文件夹
};
//---------------------------------------------------------------------------

class TDelFileThread : public ThreadBase
{
private:
    bool m_bStop;
	int	m_nThreadParam;
	int	m_nThreadPoolParam;

    void SetName();
    map<string,DELFILE_T> m_mapFileToDel; //待删除列表 
    ptime m_dtLastDel;                    //上次执行时间
    mutex m_lock;            //同步锁
    int DelOldFile(string sFilePath, string sFileTypes, int nDaysbefore, int nDelOption = 0);

	void StartThread();
	void StopThread();
protected:
    virtual void run();
public:
    TDelFileThread();
    virtual ~TDelFileThread();

	int AddTask(string sFilePath, string sFileType, int iDay, int iDelOption = 0);  //添加成功则返回1，否则返回0
	int RemoveTask(string sFilePath, string sFileType);                           //移除成功则返回1，否则返回0

    //使用以下函数进行调用
    static void StartDelFileThread();
    static void StopDelFileThread();  
    static int AddDelFileTask(string sFilePath,string sFileType,int iDay,int iDelOption = 0);      //添加成功则返回1，添加失败返回0，如果没有初化线程则返回-2 参数：文件路径，文件类型，删除天数，删除文件选项
    static int RemoveDelFileTask(string sFilePath,string sFileType);                               //移除成功则返回1，移除失败返回0，如果没有初化线程则返回-2 参数：文件路径，文件类型
};

#endif
