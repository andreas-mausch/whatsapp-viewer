#include "Settings.h"
#include "Exceptions/KeyNotFoundException.h"

Settings::Settings()
{
}

Settings::~Settings()
{
}

std::string Settings::findDisplayName(const std::string &key)
{
	try
	{
		return read("Contacts/" + key);
	}
	catch (KeyNotFoundException &exception)
	{
		return key;
	}
}
