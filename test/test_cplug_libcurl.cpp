#include "catch.hpp"
#include <vector>
#include <curl.h>
#include <cplug_filedir.h>
#include <stdio.h>
#include <log/cplug_log.h>
#include <string.h>

using namespace std;
struct memory {
	char* response;
	size_t size;
	memory()
	{
		response = NULL;
		size = 0;
	}
};
size_t httpCallBack(void* data, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;
	struct memory* mem = (struct memory*)userp;
	char* ptr = (char*)realloc(mem->response, mem->size + realsize + 1);
	if (ptr == NULL)
		return 0;

	mem->response = ptr;
	memcpy(&(mem->response[mem->size]), data, realsize);
	mem->size += realsize;
	mem->response[mem->size] = 0;

	return realsize;
}

TEST_CASE("1:Test libcurl", "[libcurl]")
{
	struct memory stBody;
	struct memory stHead;

	try
	{
		CURL* curl = curl_easy_init();
		CURLcode res;
		if (curl)
		{
			string sUrl = "http://127.0.0.1:8899/Algorithm";
			curl_easy_setopt(curl, CURLOPT_URL, sUrl.c_str());
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
			curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
			curl_easy_setopt(curl, CURLOPT_VERBOSE, 1);
			curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);

			struct curl_httppost* formpost = 0;
			struct curl_httppost* lastptr = 0;

			string sTestcurl = Cplug_FD_GetCurrentDir() + "/test_cplug_libcurl/testcurl.txt";
			Cplug_FD_CreateDirectories(Cplug_FD_GetCurrentDir() + "/test_cplug_libcurl/");
			FILE* file = fopen(sTestcurl.c_str(), "wb");
			fputs("abc", file);
			fclose(file);
			curl_formadd(&formpost, &lastptr, CURLFORM_PTRNAME, "", CURLFORM_FILE, sTestcurl.c_str(), CURLFORM_END);
			curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);

			curl_easy_setopt(curl, CURLOPT_HEADERFUNCTION, httpCallBack);
			curl_easy_setopt(curl, CURLOPT_HEADERDATA, (void*)&stHead);
			curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
			curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 10);
			curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10);
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, httpCallBack);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&stBody);

			res = curl_easy_perform(curl);
		}
		if (!stBody.response)
		{
			curl_easy_cleanup(curl);
			return;
		}
		string sBody = stBody.response;
		logInfo("¥Ú”°Body:\n%s\n", sBody.c_str());
		curl_easy_cleanup(curl);
	}
	catch (exception& e)
	{
		logError("%s throw an exception error: %s", __FUNCTION__, e.what());
	}
	catch (...)
	{
		logError("%s throw an unknow exception", __FUNCTION__);
	}
}