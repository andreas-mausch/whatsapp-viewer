#include "WhatsAppViewer.h"
#include "Platforms/Win32/GUI/MainWindow.h"

WhatsAppViewer::WhatsAppViewer()
{
	mainWindow = new MainWindow(*this);
}

WhatsAppViewer::~WhatsAppViewer()
{
	delete mainWindow;
}

bool WhatsAppViewer::tick()
{
	return mainWindow->handleMessages();
}

void WhatsAppViewer::openDatabase(const std::string &filename)
{
}

Settings &WhatsAppViewer::getSettings()
{
	return settings;
}
