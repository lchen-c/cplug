#pragma once

#include <string>
using namespace std;

enum LOGLEVEL
{
    LOG_LEVEL_TRACE,
    LOG_LEVEL_DEBUG,
    LOG_LEVEL_INFO,
    LOG_LEVEL_WARN,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_NUL
};

#define DEF_LOG_PREFIX "DEFAULT"    // 调用logXXXByPre()系列写日志接口，第一个参数不要为空串，可以用这个宏初始化

void getLogPath(string& sLogPath);
void getLogLevel(int& iLevel);
void setPrefix(const string& prefix);
void setLogPath(const string& sLogPath);
void setLogLevel(const int nLogLevel);
void setLogMode(const bool isInstantWrite);
void setStayDay(const int nDays);
void logTrace(const string& msg);
void logDebug(const string& msg);
void logInfo(const string& msg);
void logWarn(const string& msg);
void logError(const string& msg);

void logTrace(const char* msg, ...);
void logDebug(const char* msg, ...);
void logInfo(const char* msg, ...);
void logWarn(const char* msg, ...);
void logError(const char* msg, ...);
    
void logByLevel(const int nLogLevel, const string& msg);
void logTraceByPre(const string& sPrefix, const string& msg);
void logDebugByPre(const string& sPrefix, const string& msg);
void logInfoByPre(const string& sPrefix, const string& msg);
void logWarnByPre(const string& sPrefix, const string& msg);
void logErrorByPre(const string& sPrefix, const string& msg);
void logAllByLevel(const int nLogLevel, const string& msg);
