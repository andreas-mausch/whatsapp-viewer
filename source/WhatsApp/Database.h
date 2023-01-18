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

	void validate();
	bool hasTable(const std::string &tableName);
	bool hasColumn(const std::string &tableName, const std::string &columnName);

	std::string findDisplayName(Settings &settings, const std::string &key);
	int messagesCount(int chatId, int fromMe, sqlite3_stmt *res);

public:
	WhatsappDatabase(const std::string &filename);
	~WhatsappDatabase();

	void getChats(Settings &settings, std::vector<WhatsappChat *> &chats);
	void getMessages(int chatId, std::vector<WhatsappMessage *> &messages, const volatile bool &running);

};
