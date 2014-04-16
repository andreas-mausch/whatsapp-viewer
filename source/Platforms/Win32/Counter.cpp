#include <windows.h>
#include <stdio.h>
#include <string>

#include "Counter.h"
#include "StringHelper.h"

Counter::Counter()
{
	QueryPerformanceFrequency((LARGE_INTEGER *)&frequency);
	QueryPerformanceCounter((LARGE_INTEGER *)&startTime);
}

Counter::~Counter()
{
}

double Counter::getElapsedTime()
{
	long long currentTime;
	QueryPerformanceCounter((LARGE_INTEGER *)&currentTime);

	long long elapsed = currentTime - startTime;

	return static_cast<double>(elapsed) / static_cast<double>(frequency) * 1000.0;
}

void Counter::showElapsedTime(const std::string &text)
{
	WCHAR time[256];
	swprintf(time, 256, L"%f", getElapsedTime());

	std::wstring caption = strtowstr(text);
	MessageBox(NULL, time, caption.c_str(), MB_OK | MB_ICONINFORMATION);
}
