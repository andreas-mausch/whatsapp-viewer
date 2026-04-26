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
	enum SchemaVersion
	{
		SCHEMA_LEGACY,
		SCHEMA_MODERN
	};

	SQLiteDatabase database;
	SchemaVersion schemaVersion;

	void validate();
	bool hasTable(const std::string &tableName);
	bool hasColumn(const std::string &tableName, const std::string &columnName);

	std::string findDisplayName(Settings &settings, const std::string &key);
	int messagesCount(const std::string &chatId, int fromMe);
	int messagesCountLegacy(const std::string &chatId, int fromMe);
	int messagesCountModern(const std::string &chatId, int fromMe);

public:
	WhatsappDatabase(const std::string &filename);
	~WhatsappDatabase();

	void getChats(Settings &settings, std::vector<WhatsappChat *> &chats);
	void getMessages(const std::string &chatId, std::vector<WhatsappMessage *> &messages, const volatile bool &running);
	bool isModernSchema() const;

};
