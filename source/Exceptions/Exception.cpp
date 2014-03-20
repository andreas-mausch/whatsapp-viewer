#include "Exception.h"

Exception::Exception(const std::string &cause)
	: cause(cause)
{
}

const std::string &Exception::getCause() const
{
	return cause;
}
