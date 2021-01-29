#include "../log/cplug_log.h"
#include "cplug_winservice.h"
#include <string.h>

#ifdef _WIN32
#include <Windows.h> 
#define SLEEP_TIME 5000
#define SERVICE_MAXLEN 200
static char g_ServiceName[SERVICE_MAXLEN] = { 0 };
static SERVICE_STATUS ServiceStatus;
static SERVICE_STATUS_HANDLE hStatus;

bool Cplug_WS_InstallService(const string& sExePathName, const string& sServiceName, const string& sDisplayName, const string& sDescript)
{
	SC_HANDLE schSCManager, schService;
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (schSCManager == NULL)
		return false;

	schService = CreateServiceA((struct SC_HANDLE__*)schSCManager, sServiceName.c_str(), sDisplayName.c_str(),             // service name to display  
		SERVICE_ALL_ACCESS,          // desired access  
		SERVICE_WIN32_OWN_PROCESS, // service type  
		SERVICE_AUTO_START,        // start type  
		SERVICE_ERROR_NORMAL,        // error control type  
		sExePathName.c_str(),          // service's binary  
		NULL,                        // no load ordering group  
		NULL,                        // no tag identifier  
		NULL,                        // no dependencies  
		NULL,                        // LocalSystem account  
		NULL);                       // no password  

	if (schService == NULL)
		return false;
	char des[1024] = { 0 };
	std::strncpy(des, sDescript.c_str(), sDescript.size());
	SERVICE_DESCRIPTIONA sdBuf;
	sdBuf.lpDescription = des;
	ChangeServiceConfig2A(schService, SERVICE_CONFIG_DESCRIPTION, &sdBuf);
	CloseServiceHandle((struct SC_HANDLE__*)schService);
	printf("service [%s] install success\n", sServiceName.c_str());
	return true;
}


bool Cplug_WS_DeleteService(const string& sServiceName)
{
	HANDLE schSCManager;
	SC_HANDLE hService;
	schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

	if (schSCManager == NULL)
		return false;
	hService = OpenServiceA((struct SC_HANDLE__*)schSCManager, sServiceName.c_str(), SERVICE_ALL_ACCESS);
	if (hService == NULL)
		return false;
	if (DeleteService(hService) == 0)
		return false;
	if (CloseServiceHandle(hService) == 0)
		return false;
	else
	{
		printf("service [%s] uninstall success\n", sServiceName.c_str());
		return true;
	}
}


void ControlHandler(DWORD request)
{
	switch (request)
	{
	case SERVICE_CONTROL_STOP:
		logInfo("################SERVICE STOP################");
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;

	case SERVICE_CONTROL_SHUTDOWN:
		ServiceStatus.dwWin32ExitCode = 0;
		ServiceStatus.dwCurrentState = SERVICE_STOPPED;
		SetServiceStatus(hStatus, &ServiceStatus);
		return;

	default:
		break;
	}

	SetServiceStatus(hStatus, &ServiceStatus);
	return;
}

void ServiceMain(int argc, char** argv)
{
	ServiceStatus.dwServiceType             = SERVICE_WIN32;
	ServiceStatus.dwCurrentState            = SERVICE_START_PENDING;
	ServiceStatus.dwControlsAccepted        = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
	ServiceStatus.dwWin32ExitCode           = 0;
	ServiceStatus.dwServiceSpecificExitCode = 0;
	ServiceStatus.dwCheckPoint              = 0;
	ServiceStatus.dwWaitHint                = 0;

	hStatus = RegisterServiceCtrlHandlerA(g_ServiceName, (LPHANDLER_FUNCTION)ControlHandler);
	if (hStatus == (SERVICE_STATUS_HANDLE)0)
	{
		return;
	}
	ServiceStatus.dwCurrentState = SERVICE_RUNNING;
	SetServiceStatus(hStatus, &ServiceStatus);

	MEMORYSTATUS memory;
	while (ServiceStatus.dwCurrentState == SERVICE_RUNNING)
	{
		GlobalMemoryStatus(&memory);
		Sleep(SLEEP_TIME);
	}
	return;
}

bool Cplug_WS_StartSerive(const std::string& sServiceName)
{
	SC_HANDLE hSC = ::OpenSCManager(NULL, NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		return false;
	}
	SC_HANDLE hSvc = ::OpenServiceA(hSC, sServiceName.c_str(),
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		::CloseServiceHandle(hSC);
		return false;
	}
	// 获得服务的状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return false;
	}
	if (status.dwCurrentState == SERVICE_STOPPED)
	{
		// 启动服务
		if (::StartService(hSvc, NULL, NULL) == FALSE)
		{
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return false;
		}
		// 等待服务启动
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_RUNNING)
			{
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				printf("service [%s] start success\n", sServiceName.c_str());
				return true;
			}
		}
	}

	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return false;
}

bool Cplug_WS_RunSerive(const std::string& sServiceName)
{
	memset(g_ServiceName, 0, SERVICE_MAXLEN);	
	strcpy(g_ServiceName, sServiceName.c_str());
	SERVICE_TABLE_ENTRYA ServiceTable[2];
	ServiceTable[0].lpServiceName = g_ServiceName;
	ServiceTable[0].lpServiceProc = (LPSERVICE_MAIN_FUNCTIONA)ServiceMain;
	ServiceTable[1].lpServiceName = NULL;
	ServiceTable[1].lpServiceProc = NULL;
	return StartServiceCtrlDispatcherA(ServiceTable);
}

bool Cplug_WS_StopSerive(const std::string& sServiceName)
{
	SC_HANDLE hSC = ::OpenSCManager(NULL,
		NULL, GENERIC_EXECUTE);
	if (hSC == NULL)
	{
		return false;
	}
	SC_HANDLE hSvc = ::OpenServiceA(hSC, sServiceName.c_str(),
		SERVICE_START | SERVICE_QUERY_STATUS | SERVICE_STOP);
	if (hSvc == NULL)
	{
		::CloseServiceHandle(hSC);
		return false;
	}
	// 获得服务的状态
	SERVICE_STATUS status;
	if (::QueryServiceStatus(hSvc, &status) == FALSE)
	{
		::CloseServiceHandle(hSvc);
		::CloseServiceHandle(hSC);
		return false;
	}
	//如果处于停止状态则启动服务，否则停止服务。
	if (status.dwCurrentState == SERVICE_RUNNING)
	{
		// 停止服务
		if (::ControlService(hSvc,
			SERVICE_CONTROL_STOP, &status) == FALSE)
		{
			::CloseServiceHandle(hSvc);
			::CloseServiceHandle(hSC);
			return false;
		}
		// 等待服务停止
		while (::QueryServiceStatus(hSvc, &status) == TRUE)
		{
			::Sleep(status.dwWaitHint);
			if (status.dwCurrentState == SERVICE_STOPPED)
			{
				::CloseServiceHandle(hSvc);
				::CloseServiceHandle(hSC);
				printf("service [%s] stop success\n", sServiceName.c_str());
				return true;
			}
		}
	}
	::CloseServiceHandle(hSvc);
	::CloseServiceHandle(hSC);
	return false;
}

#endif
