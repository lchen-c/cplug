#include "catch.hpp"
#include "cplug_filedir.h"
#include <string.h>


TEST_CASE("1:Test File&Dir", "[FileDir]")
{
	printf("\n##################Test File&Dir##################\n");
	string newDir = Cplug_FD_GetCurrentDir() + "/test_cplug_filedir";
	REQUIRE(!Cplug_FD_FileExists(newDir.c_str()));
	REQUIRE(Cplug_FD_CreateDirectories(newDir));
	REQUIRE(Cplug_FD_FileExists(newDir.c_str()));
	REQUIRE(Cplug_FD_DirRemove(newDir.c_str()));
}
