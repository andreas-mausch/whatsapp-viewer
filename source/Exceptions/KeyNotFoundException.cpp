#include<sstream>

#include "KeyNotFoundException.h"

KeyNotFoundException::KeyNotFoundException(const std::string &reason)
	: Exception(reason)
{
}
