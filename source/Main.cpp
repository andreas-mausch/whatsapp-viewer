#ifdef _DEBUG
	#include <vld.h>
#endif

#include "Log.h"
#include "Main.h"
#include "Exceptions/Exception.h"
#include "Platforms/Win32/GUI/MainWindow.h"
#include "Platforms/Win32/SettingsRegistry.h"
#include "Platforms/Win32/StringHelper.h"

void entryPoint(const std::vector<std::string *> arguments)
{
	try
	{
		SettingsRegistry settingsRegistry;
		MainWindow mainWindow(settingsRegistry);

		if (arguments.size() > 1)
		{
			mainWindow.openDatabase(*arguments[1]);
		}

		bool run = true;
		while (run)
		{
			if (!mainWindow.handleMessages())
			{
				run = false;
			}
		}
	}
	catch (Exception &exception)
	{
		std::wstring cause = strtowstr(exception.getCause());
		MessageBox(NULL, cause.c_str(), L"Error", MB_OK | MB_ICONERROR);
	}
}
