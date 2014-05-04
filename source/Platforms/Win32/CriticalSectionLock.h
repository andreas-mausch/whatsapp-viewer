#pragma once

#include "../../Synchronization/Lock.h"

class CriticalSectionLock : public Lock
{
private:
	CRITICAL_SECTION criticalSection;

public:
	CriticalSectionLock();
	virtual ~CriticalSectionLock();

	virtual void lock();
	virtual bool tryLock();
	virtual void unlock();
};
