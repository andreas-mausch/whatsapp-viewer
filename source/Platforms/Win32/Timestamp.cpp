#include <time.h>
#include <windows.h>

#include "Timestamp.h"

WCHAR *buildTimestampString(long long timestamp)
{
    WCHAR *buffer = new WCHAR[60];
	tm date;
	timestamp /= 1000;
    localtime_s(&date, &timestamp);
	wcsftime(buffer, 60, L"%Y.%m.%d - %H:%M:%S", &date);
    return buffer;
}
