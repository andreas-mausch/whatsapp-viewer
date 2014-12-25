#pragma once

#include <windows.h>
#include <string>
#include <vector>

#include "../Libraries/SQLite/SQLiteDatabase.h"

class Settings;
class WhatsappChat;
class WhatsappMessage;

class WhatsappDatabase
{
private:
	SQLiteDatabase database;
	std::string findDisplayName(Settings &settings, const std::string &key);

public:
	WhatsappDatabase(const std::string &filename);
	~WhatsappDatabase();

	void getChats(Settings &settings, std::vector<WhatsappChat *> &chats);
	void getMessages(const std::string &chatId, std::vector<WhatsappMessage *> &messages, const volatile bool &running);

};
