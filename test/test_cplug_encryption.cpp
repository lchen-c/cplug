#include "catch.hpp"
#include <cplug_filedir.h>
#include <encryption/cplug_encryption.h>

TEST_CASE("1:Test Encryption", "[Encryption]")
{
	printf("\n##################Test Encryption##################\n");
	std::string sMd5;
	std::string str = "hello_world";
	char md5_str1[33] = { 0 };
	unsigned char p[12] = { "hello_world" };
	Cplug_EncryptionStr_MD5(p, 11, md5_str1);
	printf("[hello_world] Md5: %s\n", md5_str1);

	string sFileName = Cplug_FD_GetCurrentDir() + "/test_cplug_encryption/1.txt";
	Cplug_FD_CreateDirectories(Cplug_FD_GetCurrentDir() + "/test_cplug_encryption/");
	FILE* encryption_file = fopen(sFileName.c_str(), "wb");
	fputs("hello_world", encryption_file);
	fclose(encryption_file);
	char md5_str2[33] = { 0 };
	Cplug_EncryptionFile_MD5(sFileName.c_str(), md5_str2);
	printf("[hello_world file] Md5: %s\n", md5_str2);
}
