#include <time.h>
#include <windows.h>
#include <string>

#include "Timestamp.h"

std::string formatTimestamp(long long timestamp)
{
	long long todayTimestamp = time(NULL);

	char buffer[256];
	tm date;
	long long timestamp1000 = timestamp / 1000;
	localtime_s(&date, &timestamp1000);

	const char *formatString = "%Y.%m.%d - %H:%M:%S";

	if (isSameDay(timestamp, todayTimestamp))
	{
		// today!
		formatString = "%H:%M:%S";
	}

	strftime(buffer, 256, formatString, &date);
	return buffer;
}

std::string formatDate(long long timestamp)
{
	char buffer[256];
	tm date;
	timestamp /= 1000;
	localtime_s(&date, &timestamp);

	const char *formatString = "%Y.%m.%d";
	strftime(buffer, 256, formatString, &date);
	return buffer;
}

bool isSameDay(long long timestamp1, long long timestamp2)
{
	tm date1, date2;
	timestamp1 /= 1000;
	timestamp2 /= 1000;
	localtime_s(&date1, &timestamp1);
	localtime_s(&date2, &timestamp2);

	if (date1.tm_year == date2.tm_year &&
		date1.tm_mon == date2.tm_mon &&
		date1.tm_mday == date2.tm_mday)
	{
		return true;
	}

	return false;
}
