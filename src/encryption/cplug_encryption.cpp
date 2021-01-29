#include "cplug_encryption.h"
#include "md5.h"
#include <stdio.h>

#define READ_DATA_SIZE	1024
#define MD5_SIZE		16
#define MD5_STR_LEN		(MD5_SIZE * 2)

int Cplug_EncryptionStr_MD5(unsigned char* dest_str, unsigned int dest_len, char* md5_str)
{
    int i;
    unsigned char md5_value[MD5_SIZE];
    MD5_CTX md5;
    MD5Init(&md5);

    MD5Update(&md5, dest_str, dest_len);

    MD5Final(&md5, md5_value);

    for (i = 0; i < MD5_SIZE; i++)
    {
        snprintf(md5_str + i * 2, 2 + 1, "%02x", md5_value[i]);
    }
    return 0;
}

int Cplug_EncryptionFile_MD5(const char* file_path, char* md5_str)
{
	int i;
	int ret;
	unsigned char data[READ_DATA_SIZE];
	unsigned char md5_value[MD5_SIZE];
	MD5_CTX md5;

	FILE* fd = fopen(file_path, "rb");

	if (nullptr == fd)
	{
		perror("open");
		return -1;
	}

	MD5Init(&md5);

	while (1)
	{
		ret = fread(data, 1, READ_DATA_SIZE, fd);
		if (-1 == ret)
		{
			perror("read");
			fclose(fd);
			return -1;
		}

		MD5Update(&md5, data, ret);

		if (0 == ret || ret < READ_DATA_SIZE)
		{
			break;
		}
	}

	fclose(fd);
	MD5Final(&md5, md5_value);

	for (i = 0; i < MD5_SIZE; i++)
	{
		snprintf(md5_str + i * 2, 2 + 1, "%02x", md5_value[i]);
	}

	return 0;
}

