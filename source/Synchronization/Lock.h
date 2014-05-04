#pragma once

class Lock
{
public:
	Lock() {};
	virtual ~Lock() {};

	virtual void lock() = 0;
	virtual bool tryLock() = 0;
	virtual bool tryLockFor(int milliseconds) = 0;
	virtual bool tryLockWhile(volatile bool &condition) = 0;
	virtual void unlock() = 0;
};
