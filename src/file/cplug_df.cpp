#include "cplug_df.h"
#include "delfilethread.h"

void Cplug_DF_StartDelFileThread()
{
	TDelFileThread::StartDelFileThread();
}

void Cplug_DF_StopDelFileThread()
{
	TDelFileThread::StopDelFileThread();
}

int Cplug_DF_AddDelFileTask(string sFilePath, string sFileType, int iDay, int iDelOption)
{
	return TDelFileThread::AddDelFileTask(sFilePath, sFileType, iDay, iDelOption);
}

int Cplug_DF_RemoveDelFileTask(string sFilePath, string sFileType)
{
	return TDelFileThread::RemoveDelFileTask(sFilePath, sFileType);
}
