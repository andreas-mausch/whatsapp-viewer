#pragma once

#include "../../Settings.h"

class SettingsRegistry : public Settings
{
public:
	SettingsRegistry();
	virtual ~SettingsRegistry();

	virtual std::string read(const std::string &key);
	virtual void write(const std::string &key, const std::string &value);

};
