#pragma once

#include "Platforms/Win32/SettingsRegistry.h"

class MainWindow;

class WhatsAppViewer
{
private:
	SettingsRegistry settings;
	MainWindow *mainWindow;
public:
	WhatsAppViewer();
	~WhatsAppViewer();

	bool tick();
	void openDatabase(const std::string &filename);
	Settings &getSettings();
};
