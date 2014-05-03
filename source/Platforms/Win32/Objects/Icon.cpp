#include <windows.h>

#include "Icon.h"
#include "../../../Exceptions/Exception.h"

Icon::Icon(const TCHAR *iconName, int width, int height)
{
	handle = static_cast<HICON>(LoadImage(GetModuleHandle(NULL), iconName, IMAGE_ICON, width, height, 0));

	if (handle == NULL)
	{
		throw Exception("could not load icon");
	}
}

Icon::~Icon()
{
	DestroyIcon(handle);
}

HICON Icon::get()
{
	return handle;
}
