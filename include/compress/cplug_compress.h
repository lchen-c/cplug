#pragma once
#include <string>
#include <vector>

using namespace std;

int Cplug_CP_ZipFile(const string& sZipFileName, const vector<string>& vUnZipFile, bool bAppendFile = false, int nCompressLevel = 0);

int Cplug_CP_UnZipFile(const string& sZipFileName, const string& sUnZipFileDir);
