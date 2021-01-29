#include "catch.hpp"
#include "cplug_datetime.h"
#include <string.h>


TEST_CASE("1:Test Date&Time", "[DateTime]")
{
	printf("\n##################Test Date&Time##################\n");

	boost::posix_time::ptime now = NOW;
	printf("[SHOW] Current time: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", now).c_str());
	
	boost::posix_time::ptime nowadd1second = Cplug_DT_GetDiffDateTime(now, 1, SECOND);
	printf("[SHOW] Current time add 1 second: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", nowadd1second).c_str());

	boost::posix_time::ptime nowadd1minute = Cplug_DT_GetDiffDateTime(now, 1, MINUTE);
	printf("[SHOW] Current time add 1 minute: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", nowadd1minute).c_str());

	boost::posix_time::ptime nowadd1hour = Cplug_DT_GetDiffDateTime(now, 1, HOUR);
	printf("[SHOW] Current time add 1 hour: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", nowadd1hour).c_str());

	boost::posix_time::ptime nowadd1day = Cplug_DT_GetDiffDateTime(now, 1, DAY);
	printf("[SHOW] Current time add 1 day: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", nowadd1day).c_str());

	boost::posix_time::ptime nowadd1month = Cplug_DT_GetDiffDateTime(now, 1, MONTH);
	printf("[SHOW] Current time add 1 month: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", nowadd1month).c_str());

	boost::posix_time::ptime nowadd1year = Cplug_DT_GetDiffDateTime(now, 1, YEAR);
	printf("[SHOW] Current time add 1 year: %s\n", Cplug_DT_GetFormatStrDateTime("yyyy-MM-dd hh:mm:ss", nowadd1year).c_str());

	int nDiff = Cplug_DT_GetElapseDateTime(now, nowadd1second, SECOND);
	printf("[SHOW] Diff second: %d\n", nDiff);
}
