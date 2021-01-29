#pragma once
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;

typedef enum {SECOND, MINUTE, HOUR, DAY, MONTH, YEAR} E_DTTYPE;
//
//time_t Cplug_DT_GetDiffseconds(const SYSTEMTIME& t1, const SYSTEMTIME& t2);
//
//time_t Cplug_DT_GetDiffDays(const SYSTEMTIME& t1, const SYSTEMTIME& t2);
//

#define NOW microsec_clock::local_time()

string Cplug_DT_GetFormatStrDateTime(const string& sFormat, boost::posix_time::ptime now = NOW);

boost::posix_time::ptime Cplug_DT_GetDiffDateTime(boost::posix_time::ptime sysTime, unsigned int num, E_DTTYPE type);

int Cplug_DT_GetElapseDateTime(boost::posix_time::ptime time1, boost::posix_time::ptime time2, E_DTTYPE type);

//string Cplug_DT_FormatDateTime(string formatStr, const SYSTEMTIME& systime = Cplug_DT_GetCurrentDateTime());
