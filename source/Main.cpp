#ifdef _DEBUG
	#include <vld.h>
#endif

#include "Log.h"
#include "Main.h"
#include "Exceptions/Exception.h"
#include "Platforms/Win32/GUI/MainWindow.h"
#include "Platforms/Win32/SettingsRegistry.h"

void entryPoint()
{
	SettingsRegistry settingsRegistry;
	MainWindow mainWindow(settingsRegistry);

	bool run = true;
	while (run)
	{
		if (!mainWindow.handleMessages())
		{
			run = false;
		}
	}
}
