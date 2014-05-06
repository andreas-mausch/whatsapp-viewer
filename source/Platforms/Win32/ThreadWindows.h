#pragma once

#include "../../Thread.h"

class ThreadWindows : public Thread
{
private:
	HANDLE thread;
	static DWORD CALLBACK threadEntry(void *param);

public:
	ThreadWindows();
	virtual ~ThreadWindows();

	virtual void interrupt();
	virtual void join();

};
