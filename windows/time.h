#pragma once

struct Time
{
	Time();
	~Time() = default;

	int32_t year;
	int32_t month;
	int32_t day;
	int32_t hour;
	int32_t mins;
	int32_t secs;
	int32_t weekDay;
};

Time GetTime();