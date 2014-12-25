#include <windows.h>

#include "../../Exceptions/Exception.h"
#include "Resource.h"

void loadResource(const WCHAR *name, const WCHAR *type, unsigned char *&bytes, DWORD &size)
{
	HRSRC resource = FindResource(NULL, name, type);

	if (resource == NULL)
	{
		throw Exception("could not find resource");
	}

	HGLOBAL resourceData = LoadResource(NULL, resource);

	if (resourceData == NULL)
	{
		throw Exception("could not load resource");
	}

	bytes = reinterpret_cast<unsigned char *>(LockResource(resourceData));

	if (bytes == NULL)
	{
		throw Exception("could not lock resource");
	}

	size = SizeofResource(NULL, resource);

	if (size == 0)
	{
		throw Exception("could not get size of resource");
	}
}
