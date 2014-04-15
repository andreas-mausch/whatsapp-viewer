#include <time.h>
#include <windows.h>
#include <string>

#include "Timestamp.h"

std::string formatTimestamp(long long timestamp)
{
	char buffer[60];
	tm date;
	timestamp /= 1000;
	localtime_s(&date, &timestamp);
	strftime(buffer, 60, "%Y.%m.%d - %H:%M:%S", &date);
	return buffer;
}
