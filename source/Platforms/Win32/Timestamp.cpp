#include <time.h>
#include <windows.h>
#include <string>

#include "Timestamp.h"

std::string formatTimestamp(long long timestamp)
{
	long long todayTimestamp = time(NULL);
	tm today;
	localtime_s(&today, &todayTimestamp);

	char buffer[256];
	tm date;
	timestamp /= 1000;
	localtime_s(&date, &timestamp);

	const char *formatString = "%Y.%m.%d - %H:%M:%S";

	if (date.tm_year == today.tm_year &&
		date.tm_mon == today.tm_mon &&
		date.tm_mday == today.tm_mday)
	{
		// today!
		formatString = "%H:%M:%S";
	}

	strftime(buffer, 256, formatString, &date);
	return buffer;
}
