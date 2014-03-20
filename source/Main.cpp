#include "Log.h"
#include "Main.h"
#include "Exceptions/Exception.h"
#include "WhatsApp/Chat.h"
#include "WhatsApp/Crypt5.h"
#include "WhatsApp/Message.h"
#include "WhatsApp/Database.h"
#include "Platforms/Win32/GUI/MainWindow.h"

Log log;

void entryPoint()
{
	try
	{
		log << "WhatsApp Reader" << std::endl;

		unsigned char key[24];

		const char *accountName = "neonew.mobile@googlemail.com";
		buildKey(key, accountName);

		WhatsappDatabase whatsappDatabase("../data/msgstore.db");
		std::vector<WhatsappChat *> chats;

		whatsappDatabase.getChats(chats);

		MainWindow mainWindow(chats);

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
