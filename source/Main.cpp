#include "Log.h"
#include "Main.h"
#include "Exceptions/Exception.h"
#include "Platforms/Win32/GUI/MainWindow.h"

Log log;

void entryPoint()
{
	try
	{
		log << "WhatsApp Viewer" << std::endl;

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
	catch (Exception &exception)
	{
		log << "EXCEPTION: " << exception.getCause() << std::endl;
	}
}
