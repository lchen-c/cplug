#include "cplug_sys.h"
#include "cplug_str.h"
#include <memory>
#include "boost/lexical_cast.hpp"
#include "boost/thread.hpp"
#if defined (_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <process.h>
#include <tlhelp32.h>
#include <Psapi.h>
#include <io.h> 
#pragma comment(lib, "Version.lib")
#else
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <string>
#endif

static string g_SoftVersion = "";


std::string Cplus_Sys_GetCurrentThreadID()
{
    return boost::lexical_cast<std::string>(boost::this_thread::get_id());
}

void Cplus_Sys_SetSoftVersion(const std::string& sVersion)
{
    g_SoftVersion = sVersion;
}

string Cplus_Sys_GetSoftVersion()
{
    if (g_SoftVersion.length() != 0)
        return g_SoftVersion;
    else
        return "Unknow";
}

long long Cplus_Sys_GetExeRunTime()
{
    static boost::posix_time::ptime sTime = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::ptime eTime = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = eTime - sTime;
    return time_elapse.total_seconds();
}

long long Cplus_Sys_GetSysRunTime()
{
    return 0;
}

#if linux
static uint64_t get_res_mem_all()
{
    struct sysinfo s;
    if (0 == sysinfo(&s))
    {
        return (s.totalram * s.mem_unit) / 1024;
    }
    return 0;
}

static string read_stat_file(const char* file_name)
{
    std::string stat_info;
    FILE* fd;
    if ((fd = fopen(file_name, "r")))
    {
        stat_info.resize(1024);
        fgets(const_cast<char*>(stat_info.c_str()), 1024, fd);
        stat_info.resize(strlen(stat_info.c_str()));
        fclose(fd);
    }
    return stat_info;
}

static uint64_t get_res_mem_pid()
{
    std::string pid_meminfo = read_stat_file("/proc/self/statm");
    uint64_t virt, res;
    sscanf(pid_meminfo.c_str(), "%lu%lu", &virt, &res);
    return res * (getpagesize() / 1024);
}
#endif

string Cplus_Sys_GetMemoryInfo()
{
#if defined (_WIN32) || defined(_WIN64)
	HANDLE handle = GetCurrentProcess();
	PROCESS_MEMORY_COUNTERS pmc;
	GetProcessMemoryInfo(handle, &pmc, sizeof(pmc));
	string sLog = string("Physical memory(current/peak):") + to_string(pmc.WorkingSetSize / 1024) + "K/" + to_string(pmc.PeakWorkingSetSize / 1024) + "K, Virtual memory(current/peak):" + to_string(pmc.PagefileUsage / 1024) + "K/" + to_string(pmc.PeakPagefileUsage / 1024) + "K";
	return sLog;
#elif linux
    string sLog = string("Memory(Total mem/Current mem):") + to_string(get_res_mem_all()) + "K/" + to_string(get_res_mem_pid()) + "K";
    return sLog;
#endif
}

long long Cplus_Sys_GetThreadCnt()
{
#if defined (_WIN32) || defined(_WIN64)
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    DWORD dwThreads;
    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return FALSE;
    char szFilePath[MAX_PATH] = { 0 };
    PROCESSENTRY32 stProcessEntry32 = { 0 };
    stProcessEntry32.dwSize = sizeof(stProcessEntry32);
    BOOL bSucceed = ::Process32First(hProcessSnap, &stProcessEntry32);;
    for (;;)
    {
        if (!bSucceed)
            break;
        if (stProcessEntry32.th32ProcessID == GetCurrentProcessId())
        {
            dwThreads = stProcessEntry32.cntThreads;
            break;
        }
        bSucceed = ::Process32Next(hProcessSnap, &stProcessEntry32);
    }
    ::CloseHandle(hProcessSnap);
    return dwThreads;
#else
    int iPid = -1;
    iPid = Cplus_Sys_GetCurrentProcessID();
    if (iPid < 0)
    {
        return -1;
    }
    char szFileName[64] = { 0 };
    FILE* fd = NULL;
    char szLineBuff[512] = { 0 };
    sprintf(szFileName, "/proc/%ld/status", iPid);
    fd = fopen(szFileName, "r");
    if (fd == NULL)
    {
        printf("open file %s failed", szFileName);
        return -1;
    }
    int iThread = -1;
    while (fgets(szLineBuff, sizeof(szLineBuff), fd) != NULL)
    {
        char szName[64] = { 0 };
        int iValue = 0;
        sscanf(szLineBuff, "%s", szName);
        if (strcmp(szName, "Threads:") == 0)
        {
            sscanf(szLineBuff, "%s %d", szName, &iValue);
            iThread = iValue;
            break;
        }
    }
    fclose(fd);
    return iThread;
#endif
}

long long Cplus_Sys_GetHandleCnt()
{
#if defined (_WIN32) || defined(_WIN64)
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, GetCurrentProcessId());
    DWORD dwHandles = 0;
    GetProcessHandleCount(hProcess, &dwHandles);
    return  dwHandles;
#elif linux
    char cmd[1024] = { 0 };
    char buf[1024] = { 0 };
    char result[1024] = { 0 };

    sprintf(cmd, "ls -l /proc/%d/fd | wc -l", Cplus_Sys_GetCurrentProcessID());

    FILE* fp = NULL;

    if ((fp = popen(cmd, "r")) == NULL) {
        printf("popen error!\n");
        return -1;
    }

    while (fgets(buf, sizeof(buf), fp)) {
        strcat(result, buf);
    }
    result[strlen(result) - 1] = '\0';
    pclose(fp);
    return Cplug_Str_ToInt(result);
#endif
}

int Cplus_Sys_GetCurrentProcessID()
{
    return getpid();
}
