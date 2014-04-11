#include <time.h>
#include <windows.h>
#include <string>

#include "Timestamp.h"

WCHAR *buildTimestampStringW(long long timestamp)
{
	WCHAR *buffer = new WCHAR[60];
	tm date;
	timestamp /= 1000;
	localtime_s(&date, &timestamp);
	wcsftime(buffer, 60, L"%Y.%m.%d - %H:%M:%S", &date);
	return buffer;
}

std::string buildTimestampString(long long timestamp)
{
	char buffer[60];
	tm date;
	timestamp /= 1000;
	localtime_s(&date, &timestamp);
	strftime(buffer, 60, "%Y.%m.%d - %H:%M:%S", &date);
	return buffer;
}
