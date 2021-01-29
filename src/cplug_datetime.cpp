#include "cplug_datetime.h"
#include <iostream>
#include <sstream>

string Cplug_DT_GetFormatStrDateTime(const string& sFormat, boost::posix_time::ptime now)
{
	std::stringstream ss;
	if (sFormat.compare("yyyy-MM-dd hh:mm:ss.zzz") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%Y-%m-%d %H:%M:%S.%f"));
		ss.imbue(loc);
		ss << now;
		return string(ss.str()).substr(0, 23);
	}
	else if (sFormat.compare("yyyyMMddhhmmsszzz") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%Y%m%d%H%M%S%f"));
		ss.imbue(loc);
		ss << now;
		return string(ss.str()).substr(0, 17);
	}
	else if (sFormat.compare("yyyy-MM-dd hh:mm:ss") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%Y-%m-%d %H:%M:%S"));
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}
	else if (sFormat.compare("yyyyMMddhhmmss") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%Y%m%d%H%M%S"));
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}
	else if (sFormat.compare("yyyy-MM-dd") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%Y-%m-%d"));
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}
	else if (sFormat.compare("yyyyMMdd") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%Y%m%d"));
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}
	else if (sFormat.compare("hh:mm:ss.zzz") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%H:%M:%S.%f"));
		ss.imbue(loc);
		ss << now;
		return string(ss.str()).substr(0, 12);
	}
	else if (sFormat.compare("hh:mm:ss") == 0)
	{
		std::locale loc(ss.getloc(), new time_facet("%H:%M:%S"));
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}
	else
	{
		std::locale loc(ss.getloc(), new time_facet("%Y-%m-%d %H:%M:%S"));
		ss.imbue(loc);
		ss << now;
		return ss.str();
	}
}

boost::posix_time::ptime Cplug_DT_GetDiffDateTime(boost::posix_time::ptime time, unsigned int num, E_DTTYPE type)
{
	switch (type)
	{
	case SECOND:
	{
		return time + seconds(num);
	}
	case MINUTE:
	{
		return time + minutes(num);

	}
	case HOUR:
	{
		return time + hours(num);
	}
	case DAY:
	{
		return time + days(num);
	}
	case MONTH:
	{
		return time + months(num);
	}
	case YEAR:
	{
		return time + years(num);
	}
	default:
		return boost::posix_time::ptime();
	}
}

int Cplug_DT_GetElapseDateTime(boost::posix_time::ptime time1, boost::posix_time::ptime time2, E_DTTYPE type)
{
	boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse = time1 - time2;
	int seconds = time_elapse.total_seconds();
	switch (type)
	{
	case SECOND:
	{
		return seconds;
	}
	case MINUTE:
	{
		return seconds / 60;
	}
	case HOUR:
	{
		return seconds / 3600;

	}
	case DAY:
	{
		return (seconds / 3600 * 24);
	}
	default:
		return 0;
	}
}
