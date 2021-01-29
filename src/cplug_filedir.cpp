#include "cplug_filedir.h"
#include <boost/filesystem/path.hpp> 
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem.hpp>
#include "log/cplug_log.h"
using namespace boost::filesystem;

static string g_CurrentDir = "";

void Cplug_FD_SetCurrentDir(const std::string& dir)
{
	g_CurrentDir = dir;
}

string Cplug_FD_GetCurrentDir()
{
	if (g_CurrentDir.empty())
	{
		return boost::filesystem::initial_path<boost::filesystem::path>().string();
	}
	else
	{
		return g_CurrentDir;
	}
}

bool Cplug_FD_CreateDirectories(const std::string& dir)
{
	path d(dir);
	if (!boost::filesystem::is_directory(d))
	{
		if (boost::filesystem::create_directories(d))
		{
			return true;
		}
		else
		{
			logError("create directories [%s] failed", dir.c_str());
			return false;
		}
	}
	return true;
}

bool Cplug_FD_DirExists(const std::string& dir)
{
	return boost::filesystem::is_directory(boost::filesystem::path(dir));
}

bool Cplug_FD_DirRemove(const std::string& dir, int type)
{
	if (0 == type)
	{
		return boost::filesystem::remove(path(dir));
	}
	else
	{
		return boost::filesystem::remove_all(path(dir));
	}
}

bool Cplug_FD_FileExists(const std::string& file)
{
	return boost::filesystem::exists(boost::filesystem::path(file));
}

bool Cplug_FD_FileRemove(const std::string& file)
{
	return boost::filesystem::remove(path(file));
}

static void Cplug_FD_FindReplace(string &srcStr, const string& subStr, const string& rpStr)
{
    size_t pos = srcStr.find(subStr);
    while (pos != -1)
    {
        srcStr.replace(pos, subStr.length(), rpStr);
        pos = srcStr.find(subStr);
    }
}

string Cplug_FD_GetDirFromFile(const string& sFilePathName)
{
	string sCopyFileName = sFilePathName;
    Cplug_FD_FindReplace(sCopyFileName, "\\", "/");
    size_t pos = sCopyFileName.find_last_of("/");
    return sCopyFileName.substr(0, pos);
}

string Cplug_FD_GetFileNameFromFile(const string& sFilePathName)
{
	string sCopyFileName = sFilePathName;
	Cplug_FD_FindReplace(sCopyFileName, "\\", "/");
	size_t pos = sCopyFileName.find_last_of("/");
	return sCopyFileName.substr(pos + 1, sCopyFileName.length() - pos - 1);
}

string Cplug_FD_SpliceDirAndFileName(const string& sDirName, const string& sFileName)
{
	string sCopyDirName = sDirName;
	Cplug_FD_FindReplace(sCopyDirName, "\\", "/");
	size_t pos = sCopyDirName.find_last_of("/");
	if (pos != sCopyDirName.length() - 1)
		sCopyDirName += "/";
	return sCopyDirName + sFileName;
}

bool Cplug_FD_CopyFile(const string& sFrom, const string& sTo)
{
	return boost::filesystem::copy_file(path(sFrom), path(sTo));
}
