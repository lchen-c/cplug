#pragma once
#include <string>

bool Cplug_WS_InstallService(const string& sExePathName, const string& sServiceName, const string& sDisplayName, const string& sDescript);

bool Cplug_WS_StartSerive(const std::string& sServiceName);

bool Cplug_WS_RunSerive(const std::string& sServiceName);

bool Cplug_WS_StopSerive(const std::string& sServiceName);

bool Cplug_WS_DeleteService(const string& sServiceName);


