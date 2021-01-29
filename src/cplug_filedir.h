#pragma once
#include <string>
#include <vector>

using namespace std;

//DIR
void Cplug_FD_SetCurrentDir(const std::string& dir);

string Cplug_FD_GetCurrentDir();

bool Cplug_FD_CreateDirectories(const std::string& dir);

bool Cplug_FD_DirExists(const std::string& dir); //路径是否存在

//type = 0 remove empty; type = 1 remove all
bool Cplug_FD_DirRemove(const std::string& dir, int type = 0);

//FILE
bool Cplug_FD_FileExists(const std::string& file);

bool Cplug_FD_FileRemove(const std::string& file);

string Cplug_FD_GetDirFromFile(const string &sFilePathName);

string Cplug_FD_GetFileNameFromFile(const string& sFilePathName);

string Cplug_FD_SpliceDirAndFileName(const string& sDirName, const string& sFileName);

bool Cplug_FD_CopyFile(const string& sFrom, const string& sTo);