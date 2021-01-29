#include "catch.hpp"
#include <cplug_filedir.h>
#include <string.h>
#include <compress/cplug_compress.h>


TEST_CASE("1:Test Compress", "[Compress]")
{
	printf("\n##################Test Compress##################\n");

#if defined (_WIN32) || defined(_WIN64)
	string sUnZipDir = Cplug_FD_GetCurrentDir() + "/test_cplug_compress/unzip/";
	string sZipDir = Cplug_FD_GetCurrentDir() + "/test_cplug_compress/zip/";
	Cplug_FD_CreateDirectories(sUnZipDir);
	Cplug_FD_CreateDirectories(sZipDir);

	vector<string> vUnZipFile;
	string sUnZip1 = Cplug_FD_GetCurrentDir() + "/test_cplug_compress/1.txt";
	string sUnZip2 = Cplug_FD_GetCurrentDir() + "/test_cplug_compress/2.txt";

	FILE* UnZipFile1 = fopen(sUnZip1.c_str(), "wb");
	fputs("abc", UnZipFile1);
	fclose(UnZipFile1);
	FILE* UnZipFile2 = fopen(sUnZip2.c_str(), "wb");
	fputs("123", UnZipFile2);
	fclose(UnZipFile2);

	vUnZipFile.push_back(sUnZip1);
	vUnZipFile.push_back(sUnZip2);
	string sZipFileName = sZipDir + "testzipfile.zip";

	Cplug_CP_ZipFile(sZipFileName, vUnZipFile, false, 0);
	Cplug_CP_UnZipFile(sZipFileName, sUnZipDir);
#endif
}
