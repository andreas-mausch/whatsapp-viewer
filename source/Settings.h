#pragma once

#include <string>

class Settings
{
public:
	Settings() {}
	virtual ~Settings() {}

	virtual std::string read(const std::string &key) = 0;
	virtual void write(const std::string &key, const std::string &value) = 0;

};
