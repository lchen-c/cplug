#pragma once


#include <string>
#include <vector>

using namespace std;

string Cplug_Str_ToUpper(string& src);

string Cplug_Str_ToLower(string& str);

int Cplug_Str_ToInt(const string& str);

double Cplug_Str_ToDouble(const string& str);

string Cplug_Str_FromInt(int nNum);

string Cplug_Str_FromDouble(double dNum);

//void Cplug_Str_Split(const string& src, const string& finstr, vector<std::string>& vec);
//
//int Cplug_Str_IndexKmp(const char* src, const char* cmp, int pos, int size_s, int size_c);
//
//void Cplug_Str_FindReplace(string& src, const string& sub, const string& rpstr);
//
wstring Cplug_Str_StrToWstr(const std::string& str);