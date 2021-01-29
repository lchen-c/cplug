#include "cplug_str.h"
#include <boost/lexical_cast.hpp>     
using namespace std;
using namespace boost;

#ifdef _WIN32
#include <Windows.h>
#endif

#if defined (_WIN32) || defined(_WIN64)
wstring Cplug_Str_StrToWstr(const std::string& str)
{
	int num = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, NULL, 0);
	wchar_t* wide = new wchar_t[num];
	auto_ptr<wchar_t> ptr(wide);
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wide, num);
	std::wstring w_str(wide);
	return w_str;
}
#else
wstring Cplug_Str_StrToWstr(const std::string& str)
{
	const int MAX_PATH = 1024;
	wchar_t wstrBuf[MAX_PATH];
	setlocale(LC_CTYPE, "en_US.UTF-8");
	mbstowcs(wstrBuf, str.c_str(), MAX_PATH - 1);
	wstrBuf[MAX_PATH - 1] = 0;
	return std::wstring(wstrBuf);
}
#endif

string Cplug_Str_ToUpper(string& src)
{
	transform(src.begin(), src.end(), src.begin(), ::toupper);
	return src;
}

string Cplug_Str_ToLower(string& str)
{
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

int Cplug_Str_ToInt(const string& str)
{
	return lexical_cast<int>(str);
}

double Cplug_Str_ToDouble(const string& str)
{
	return lexical_cast<double>(str);
}

string Cplug_Str_FromInt(int nNum)
{
	return lexical_cast<string>(nNum);
}

string Cplug_Str_FromDouble(double dNum)
{
	return lexical_cast<string>(dNum);
}
