#include "catch.hpp"
#include "cplug_filedir.h"
#include <string>
#include <compress/cplug_compress.h>
#include <aws/cplug_aws.h>

using namespace std;

TEST_CASE("1:Test AWSS3", "[AWSS3]")
{
	printf("\n##################Test AWSS3##################\n");
	Cplug_AWS_InitAwsSdk("oos-cn.ctyunapi.cn", "d1a6bd294b37b17f5180", "4e9083e9b580b76dc75c6f2a0790c6ad305bf199", "cn");
	Aws::Vector<Bucket> buckets = Cplug_AWS_ListBucket();
	std::cout << "[list bucket]" << std::endl;
	for (auto it = buckets.begin(); it != buckets.end(); it++)
	{
		std::cout << it->GetName() << std::endl;
	}

	std::cout << "[put object]" << std::endl;
	string sAws_putfile = Cplug_FD_GetCurrentDir() + "/test_cplug_awss3/Aws_putfile.txt";
	Cplug_FD_CreateDirectories(Cplug_FD_GetCurrentDir() + "/test_cplug_awss3");
	FILE* file = fopen(sAws_putfile.c_str(), "wb");
	fputs("abc123", file);
	fclose(file);
	Cplug_AWS_PutObject("ultra-data", Cplug_FD_GetFileNameFromFile(sAws_putfile), sAws_putfile);

	std::cout << "[list object]" << std::endl;
	Aws::Vector<Object> objects = Cplug_AWS_ListObject("ultra-data");
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		std::cout << it->GetKey() << std::endl;
	}

	std::cout << "[get object]" << std::endl;
	string sAws_downdir = Cplug_FD_GetCurrentDir() + "/test_cplug_awss3";
	REQUIRE(Cplug_AWS_GetObject("ultra-data", "Aws_putfile.txt", sAws_downdir, "Aws_downfile.txt"));

	std::cout << "[delete object]" << std::endl;
	REQUIRE(Cplug_AWS_DelObject("ultra-data", "Aws_putfile.txt"));

	std::cout << "[list object]" << std::endl;
	objects = Cplug_AWS_ListObject("ultra-data");
	for (auto it = objects.begin(); it != objects.end(); it++)
	{
		std::cout << it->GetKey() << std::endl;
	}

	Cplug_AWS_DeInitAwsSdk();
}