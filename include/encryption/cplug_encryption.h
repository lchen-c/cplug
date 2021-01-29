#pragma once
#include <string>

using namespace std;

int Cplug_EncryptionStr_MD5(unsigned char* dest_str, unsigned int dest_len, char* md5_str);

int Cplug_EncryptionFile_MD5(const char* file_path, char* md5_str);