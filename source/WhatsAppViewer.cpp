#include "WhatsAppViewer.h"
#include "Platforms/Win32/GUI/MainWindow.h"
#include "Platforms/Win32/ImageDecoder.h"

WhatsAppViewer::WhatsAppViewer()
{
	CoInitialize(NULL);

	imageDecoder = new ImageDecoder();
	mainWindow = new MainWindow(*this, *imageDecoder);
}

WhatsAppViewer::~WhatsAppViewer()
{
	delete mainWindow;
	delete imageDecoder;

	CoUninitialize();
}

bool WhatsAppViewer::tick()
{
	return mainWindow->handleMessages();
}

void WhatsAppViewer::openDatabase(const std::string &filename)
{
	mainWindow->openDatabase(filename);
}

Settings &WhatsAppViewer::getSettings()
{
	return settings;
}
