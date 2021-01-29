#include "catch.hpp"
#include "cplug_filedir.h"
#include <log/cplug_log.h>
#include <thread>
#include "file/cplug_df.h"

TEST_CASE("1:Delete File", "[DelFile]")
{
	printf("\n##################Delete File##################\n");

	Cplug_DF_StartDelFileThread();
	string sLogDir = Cplug_FD_GetCurrentDir() + "/Log";
	REQUIRE(0 == Cplug_DF_AddDelFileTask(sLogDir, "^testlog_[0-9]{8}.log", 1, 2));
	std::this_thread::sleep_for(chrono::seconds(2));
}
