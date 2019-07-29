#include <string>
#include <vector>

#include "CLI.h"
#include "Main.h"
#include "WhatsAppViewer.h"
#include "Exceptions/Exception.h"
#include "Platforms/Win32/StringHelper.h"

void entryPoint(const std::vector<std::string *> arguments)
{
	try
	{
		if (handleCommandLineArguments(arguments))
		{
			return;
		}

		WhatsAppViewer whatsAppViewer;

		if (arguments.size() > 1)
		{
			whatsAppViewer.openDatabase(*arguments[1]);
		}

		bool run = true;
		while (run)
		{
			if (!whatsAppViewer.tick())
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
