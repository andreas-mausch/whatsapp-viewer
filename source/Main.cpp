#include "Log.h"
#include "Main.h"
#include "Exceptions/Exception.h"
#include "Platforms/Win32/GUI/MainWindow.h"

void entryPoint()
{
	MainWindow mainWindow;

	bool run = true;
	while (run)
	{
		if (!mainWindow.handleMessages())
		{
			run = false;
		}
	}
}
