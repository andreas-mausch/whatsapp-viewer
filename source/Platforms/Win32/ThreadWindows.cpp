#include <windows.h>

#include "ThreadWindows.h"

ThreadWindows::ThreadWindows()
{
	running = true;
	thread = CreateThread(NULL, 0, threadEntry, this, 0, NULL);
}

ThreadWindows::~ThreadWindows()
{
	CloseHandle(thread);
}

void ThreadWindows::interrupt()
{
	running = false;
}

void ThreadWindows::join()
{
	WaitForSingleObject(thread, INFINITE);
}

DWORD CALLBACK ThreadWindows::threadEntry(void *param)
{
	ThreadWindows *threadWindows = reinterpret_cast<ThreadWindows *>(param);
	threadWindows->run();
	return 0;
}
