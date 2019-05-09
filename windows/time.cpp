#include "stdafx.h"
#include "time.h"

Time::Time()
{
	time_t tt;
	time(&tt);
	tm TM = *localtime(&tt);

	year = TM.tm_year + 1900;
	month = TM.tm_mon;
	day = TM.tm_mday;
	hour = TM.tm_hour;
	mins = TM.tm_min;
	secs = TM.tm_sec;
	weekDay = TM.tm_wday;
}

Time GetTime()
{
	return Time();
}