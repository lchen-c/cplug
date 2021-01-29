#pragma once
#include <string>

using namespace std;

void Cplug_DF_StartDelFileThread();

void Cplug_DF_StopDelFileThread();

int Cplug_DF_AddDelFileTask(string sFilePath, string sFileType, int iDay, int iDelOption = 0); 

int Cplug_DF_RemoveDelFileTask(string sFilePath, string sFileType);