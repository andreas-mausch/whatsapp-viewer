#include <iostream>
#include <sstream>

#include "../Exceptions/Exception.h"
#include "../SQLite/sqlite3.h"
#include "Chat.h"
#include "Message.h"
#include "Database.h"

std::string readString(sqlite3_stmt *res, int column)
{
	const unsigned char *text = sqlite3_column_text(res, column);
	if (text == NULL)
	{
		return "";
	}

	return reinterpret_cast<const char *>(text);
}

WhatsappDatabase::WhatsappDatabase(const std::string &filename)
{
	if(sqlite3_open(filename.c_str(), &sqLiteDatabase) != SQLITE_OK)
	{
		throw Exception("Could not open SQLite database");
	}
}

WhatsappDatabase::~WhatsappDatabase()
{
	sqlite3_close(sqLiteDatabase);
}

void WhatsappDatabase::getChats(std::vector<WhatsappChat*> &chats)
{
	sqlite3_stmt *res;
	const char *tail;
	int error = sqlite3_prepare_v2(sqLiteDatabase,
        "SELECT * FROM chat_list",
        1000, &res, &tail);

	if (error != SQLITE_OK)
	{
		throw Exception("Could not load chat list");
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string key = readString(res, 1);
		std::string subject = readString(res, 3);
		long long creation = sqlite3_column_int64(res, 4);

		WhatsappChat *chat = new WhatsappChat(*this, key, subject, creation);
		chats.push_back(chat);
	}
}

void WhatsappDatabase::getMessages(const std::string &chatId, std::vector<WhatsappMessage*> &messages)
{
	std::ostringstream query;
	query << "SELECT * FROM messages where key_remote_jid = '" << chatId << "' order by timestamp asc";

	sqlite3_stmt *res;
	const char *tail;
	int error = sqlite3_prepare_v2(sqLiteDatabase, query.str().c_str(), 1000, &res, &tail);

	if (error != SQLITE_OK)
	{
		throw Exception("Could not load messages");
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string chatId = readString(res, 1);
		int fromMe = sqlite3_column_int(res, 2);
		int status = sqlite3_column_int(res, 3);
		std::string data = readString(res, 6);
		long long timestamp = sqlite3_column_int64(res, 7);

		WhatsappMessage *message = new WhatsappMessage(chatId, fromMe == 1, status, data, timestamp, 0, 0, "", "", 0);
		messages.push_back(message);
	}
}
