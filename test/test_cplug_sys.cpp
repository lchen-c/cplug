#include "catch.hpp"
#include "cplug_sys.h"
#include <log/cplug_log.h>
#include <thread>
#include "cplug_str.h"

#if linux
#include <sys/types.h>
#include <sys/sysinfo.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <utility>
#include <vector>
#endif

TEST_CASE("1:Test sys", "[SYS]")
{
    printf("current thread id [%s]\n", Cplus_Sys_GetCurrentThreadID().c_str());
    printf("current process runtime [%d s]\n", Cplus_Sys_GetExeRunTime());
    printf("current handle [%d]\n", Cplus_Sys_GetHandleCnt());
    std::this_thread::sleep_for(chrono::seconds(2));
    printf("current process runtime [%d s]\n", Cplus_Sys_GetExeRunTime());
    printf("current process threadcnt [%d]\n", Cplus_Sys_GetThreadCnt());

}