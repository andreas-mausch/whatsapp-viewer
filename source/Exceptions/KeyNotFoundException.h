#pragma once

#include "Exception.h"

class KeyNotFoundException : public Exception
{
public:
	KeyNotFoundException(const std::string &reason);
};
