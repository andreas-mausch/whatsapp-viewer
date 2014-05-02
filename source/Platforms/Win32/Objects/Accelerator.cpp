#include <windows.h>

#include "Accelerator.h"
#include "../../../Exceptions/Exception.h"

Accelerator::Accelerator(const TCHAR *tableName)
{
	handle = LoadAccelerators(GetModuleHandle(NULL), tableName);

	if (handle == NULL)
	{
		throw Exception("could not load accelerators");
	}
}

Accelerator::~Accelerator()
{
}

HACCEL Accelerator::get()
{
	return handle;
}
