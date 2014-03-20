#pragma once

#include <string>

class Exception
{
private:
	std::string cause;
public:
	Exception(const std::string &cause);
	const std::string &getCause() const;
};
