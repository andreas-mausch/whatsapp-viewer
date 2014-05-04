#include <windows.h>

#include "CriticalSectionLock.h"

CriticalSectionLock::CriticalSectionLock()
{
	InitializeCriticalSection(&criticalSection);
}

CriticalSectionLock::~CriticalSectionLock()
{
	DeleteCriticalSection(&criticalSection);
}

void CriticalSectionLock::lock()
{
	EnterCriticalSection(&criticalSection);
}

bool CriticalSectionLock::tryLock()
{
	if (TryEnterCriticalSection(&criticalSection))
	{
		return true;
	}

	return false;
}

bool CriticalSectionLock::tryLockFor(int milliseconds)
{
	if (tryLock())
	{
		return true;
	}

	int remainingWaitTime = milliseconds;
	int startTime = GetTickCount();

	while (WaitForSingleObject(criticalSection.LockSemaphore, remainingWaitTime) == WAIT_OBJECT_0)
	{
		if (tryLock())
		{
			return true;
		}

		remainingWaitTime -= GetTickCount() - startTime;
		startTime = GetTickCount();
	}

	return false;
}

bool CriticalSectionLock::tryLockWhile(volatile bool &condition)
{
	while (condition)
	{
		if (tryLock())
		{
			return true;
		}

		Sleep(10);
	}

	return false;
}

void CriticalSectionLock::unlock()
{
	LeaveCriticalSection(&criticalSection);
}
