#include <windows.h>

#include "Handle.h"

Handle::Handle(HANDLE handle)
	: handle(handle)
{
}

Handle::~Handle()
{
	DeleteObject(handle);
}
