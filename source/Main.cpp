#include <windows.h>

#include "Log.h"
#include "Main.h"
#include "WhatsApp/Chat.h"
#include "WhatsApp/Crypt5.h"
#include "WhatsApp/Message.h"
#include "WhatsApp/Database.h"

Log log;

int CALLBACK WinMain(HINSTANCE instance, HINSTANCE previousInstance, LPSTR commandLine, int commandShow)
{
	log << "WhatsApp Reader" << std::endl;

	unsigned char key[24];

	const char *accountName = "neonew.mobile@googlemail.com";
	buildKey(key, accountName);

	WhatsappDatabase whatsappDatabase("../data/msgstore.db");
	std::vector<WhatsappChat *> chats;

	whatsappDatabase.getChats(chats);

	log << "Chat[0]: " << chats[0]->getKey() << std::endl;

	std::vector<WhatsappMessage *> messages = chats[0]->getMessages();
	for(std::vector<WhatsappMessage*>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		log << "Message: " << (*it)->getData() << std::endl;
	}

	log << std::endl;
	log << "Chat[10]: " << chats[15]->getKey() << std::endl;
	messages = chats[15]->getMessages();
	for(std::vector<WhatsappMessage*>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		log << "Message: " << (*it)->getData() << std::endl;
	}

	return 0;
}
