#pragma once

#include "../Handle.h"

class Accelerator
{
private:
	HACCEL handle;
public:
	Accelerator(const TCHAR *tableName);
	~Accelerator();

	HACCEL get();
};
