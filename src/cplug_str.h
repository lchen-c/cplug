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

wstring Cplug_Str_StrToWstr(const std::string& str);