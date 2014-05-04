#include "Locked.h"
#include "Lock.h"

Locked::Locked(Lock &lock)
	: lock(lock)
{
	lock.lock();
}

Locked::~Locked()
{
	lock.unlock();
}
