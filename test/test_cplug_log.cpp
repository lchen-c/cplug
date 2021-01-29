#include "catch.hpp"
#include "cplug_filedir.h"
#include <log/cplug_log.h>
#include <file/cplug_df.h>
#include <thread>
#include <cplug_str.h>

TEST_CASE("1:Test Log", "[Log]")
{
	printf("\n##################Test Log##################\n");

	Cplug_DF_StartDelFileThread();
	string sLogDir = Cplug_FD_GetCurrentDir() + "/Log";
	Cplug_DF_AddDelFileTask(sLogDir, "^testlog_[0-9]{8}.log", 1, 2);

	setPrefix("testlog");
	setLogLevel(2);
	setStayDay(10);

	logInfo("Test Log Start ...");
	logInfo("≤‚ ‘»’÷æ∆Ù∂Ø ...");

	std::this_thread::sleep_for(chrono::seconds(100));
}
