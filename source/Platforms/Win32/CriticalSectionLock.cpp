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

void CriticalSectionLock::unlock()
{
	LeaveCriticalSection(&criticalSection);
}
