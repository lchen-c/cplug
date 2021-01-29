#include "delfilethread.h"
#include "../cplug_datetime.h"
#include "../cplug_filedir.h"
#include <boost/filesystem.hpp>
#include <boost/regex.hpp>
#include "../cplug_str.h"


#if defined (_WIN32) || defined(_WIN64)
#include <Windows.h>        
#endif

using namespace std;
namespace fs = boost::filesystem;

TDelFileThread * gvDelFileThread = NULL;     

TDelFileThread::TDelFileThread()
{
    m_dtLastDel = Cplug_DT_GetDiffDateTime(NOW, -1, DAY);
}              
                         
TDelFileThread::~TDelFileThread()
{  
}

//开启线程
void TDelFileThread::StartThread()
{
    m_bStop = false;
    start();
}

//停止线程
void TDelFileThread::StopThread()
{
    m_bStop = true;
}

void TDelFileThread::SetName()
{

}

void  TDelFileThread::run()
{
    std::this_thread::sleep_for(chrono::seconds(1));
    SetName();
    while (m_bStop == false)
    {
        try
        {
            map<string, DELFILE_T> mapFileToDel;
			{
				lock_guard<mutex> lock(m_lock);
				try
				{
					ptime dtNow = NOW;
					if (dtNow.date().day() != m_dtLastDel.date().day())
					{
						m_dtLastDel = dtNow;
						mapFileToDel = m_mapFileToDel;
					}
				}
				catch (...)
				{
				}
			}
            if (mapFileToDel.size()>0)
            {
                map<string, DELFILE_T>::iterator it = mapFileToDel.begin();
                while (it != mapFileToDel.end())
                {
                    DelOldFile(it->second.sFilePath, it->second.sFileType, it->second.iDelDay, it->second.iDelOption);
                    ++it;
                }
            }
        }
        catch (...)
        {
            logError("Clear thread file error");
        }

        //每5分钟执行一次检测
        for(int i = 0; i<5*60; i++)
        {
            if(false == m_bStop)
            {
                std::this_thread::sleep_for(chrono::seconds(1));
            }
            else
            {
                break;
            }
        }
    }
}

#if linux
typedef struct _SYSTEMTIME {
    int wYear;
    int wMonth;
    int wDayOfWeek;
    int wDay;
    int wHour;
    int wMinute;
    int wSecond;
    int wMilliseconds;
    _SYSTEMTIME()
    {
        wYear = 0;
        wMonth = 0;
        wDayOfWeek = 0;
        wDay = 0;
        wHour = 0;
        wMinute = 0;
        wSecond = 0;
        wMilliseconds = 0;
    }
} SYSTEMTIME;
#endif
bool checkFileOverdue(string& sFileName, int nDaysbefore)
{
#if defined (_WIN32) || defined(_WIN64)
	FILETIME ftCreate, ftModify, ftAccess, ftLocal;
	std::string strCreateTime, strModifyTime, strAccessTime;

	HANDLE hFile = CreateFileA(sFileName.c_str(), GENERIC_READ,
		FILE_SHARE_READ,
		NULL,
		OPEN_EXISTING,
		FILE_FLAG_BACKUP_SEMANTICS,
		NULL);

	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftModify))
	{
		CloseHandle(hFile);
		return false;
	}
	CloseHandle(hFile);

	SYSTEMTIME createTime;
	ZeroMemory(&createTime, sizeof(SYSTEMTIME));
	FileTimeToLocalFileTime(&ftCreate, &ftLocal);
	FileTimeToSystemTime(&ftLocal, &createTime);
#else
    SYSTEMTIME createTime;
    string sName = Cplug_FD_GetFileNameFromFile(sFileName);
    int index = sName.find_last_of('.');
    string sDate = sName.substr(index - 8, 8);
    createTime.wYear = Cplug_Str_ToInt(sDate.substr(0, 4));
    createTime.wMonth = Cplug_Str_ToInt(sDate.substr(4, 2));
    createTime.wDay = Cplug_Str_ToInt(sDate.substr(6, 2));
#endif
    char sCreateDateTime[20] = { 0 };
    snprintf(sCreateDateTime, 20, "%04d-%02d-%02d %02d:%02d:%02d", createTime.wYear, createTime.wMonth, createTime.wDay, createTime.wHour, createTime.wMinute, createTime.wSecond);

	ptime time1 = time_from_string(sCreateDateTime);
	ptime time2 = microsec_clock::local_time();
	boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = time2 - time1;
	int seconds = time_elapse.total_seconds();
	int diffDays = seconds / (3600 * 24);
	if (diffDays > nDaysbefore)
		return true;
	else
		return false;
}

int TDelFileThread::DelOldFile(string sFilePath, string sFileTypes, int nDaysbefore, int nDelOption)
{
    try
    {
        if (!Cplug_FD_DirExists(sFilePath))
        {
            return -1;
        }
        if (nDaysbefore <= 0)
            return -1;

        boost::regex reg(sFileTypes.c_str());

        boost::filesystem::path myPath(sFilePath);
        boost::filesystem::directory_iterator  endIter;

        for (boost::filesystem::directory_iterator iter(myPath); iter != endIter; iter++)
        {
            boost::filesystem::path p = iter->path();
            string sPath = p.string();
            if (boost::filesystem::is_directory(*iter))
            {
                if (nDelOption > 0)
                    DelOldFile(sPath, sFileTypes, nDaysbefore, nDelOption);
                if (nDelOption == 2)
                {
                    boost::filesystem::remove(p);
                }
            }
            else if (boost::regex_match(p.filename().string(), reg))
            {
                if (checkFileOverdue(sPath, nDaysbefore))
                {
                    boost::filesystem::remove(iter->path());
                }
            }
        }

        return 0;
    }
    catch (...)
    {
        logError("DelOldFile error");
        return -1;
    }
}

int TDelFileThread::AddTask(string sFilePath,string sFileType,int iDay,int iDelOption)
{
    try
    {
		if (!Cplug_FD_DirExists(sFilePath))            //路径是否存在
        {
            return -1;
        }
        string sPathToSearch = "";
    
        if(sFilePath[sFilePath.length()-1] != '/')
            sPathToSearch = sFilePath + "/";
        else 
            sPathToSearch = sFilePath;

        DELFILE_T tmp;
        tmp.sFilePath = sPathToSearch;
        tmp.sFileType = sFileType;
        tmp.iDelDay = iDay;
        tmp.iDelOption = iDelOption;
    
        {
            lock_guard<mutex> lock(m_lock);
            try
            {
                m_mapFileToDel[sPathToSearch + sFileType] = tmp;
                m_dtLastDel = Cplug_DT_GetDiffDateTime(NOW, -1, DAY); 
            }
            catch (...)
            {
            }
        }    
    }
    catch(...)
    {
        logError("Start delete file thread failed!");
    }
    return 0;
}    
 
int TDelFileThread::RemoveTask(string sFilePath,string sFileType)
{
    string sPathToSearch = "";
    
    if(sFilePath[sFilePath.length()-1]!='/')
        sPathToSearch = sFilePath + "/";
    else 
        sPathToSearch = sFilePath;

    int iRc = 0;
    {
        lock_guard<mutex> lock(m_lock);
        if (m_mapFileToDel.count(sPathToSearch + sFileType))
        {
            m_mapFileToDel.erase(sPathToSearch + sFileType);
            iRc = 1;
            logInfo("Stop task:%s,%s", sFilePath.c_str(), sFileType.c_str());
        }
        else
            iRc = 0;
    }
    return iRc;
}                             

void TDelFileThread::StartDelFileThread()
{
    try
    {
		if (gvDelFileThread == NULL)
		{
            gvDelFileThread = new TDelFileThread();
			gvDelFileThread->StartThread();
		}	
    }   
    catch(...)
    {
        logError("Stop delete file thread!");
    }
}                                    
 
void TDelFileThread::StopDelFileThread()
{
    try
    {
        if(gvDelFileThread)
        {
            gvDelFileThread->StopThread();
            delete gvDelFileThread;
            gvDelFileThread = NULL;
        }
    }
    catch(...)
    {
        logError("Start delete file thread!");
    }
}                                                 
 
int TDelFileThread::AddDelFileTask(string sFilePath,string sFileType,int iDay,int iDelOption)
{
    logInfo("Add file delete task:%s,%s,%d", sFilePath.c_str(), sFileType.c_str(), iDay);
    if(gvDelFileThread)
        return gvDelFileThread->AddTask(sFilePath,sFileType,iDay,iDelOption);
    else return -2;
}             
   
int TDelFileThread::RemoveDelFileTask(string sFilePath,string sFileType)
{
    if(gvDelFileThread)
        return gvDelFileThread->RemoveTask(sFilePath,sFileType);
    else return -2;
}             
 
