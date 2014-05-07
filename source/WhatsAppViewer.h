#pragma once

#include "Platforms/Win32/SettingsRegistry.h"

class ImageDecoder;
class MainWindow;

class WhatsAppViewer
{
private:
	SettingsRegistry settings;
	ImageDecoder *imageDecoder;
	MainWindow *mainWindow;

public:
	WhatsAppViewer();
	~WhatsAppViewer();

	bool tick();
	void openDatabase(const std::string &filename);
	Settings &getSettings();
};
