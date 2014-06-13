#include <iostream>
#include <sstream>

#include "../Exceptions/Exception.h"
#include "../Exceptions/SQLiteException.h"
#include "../SQLite/sqlite3.h"
#include "Chat.h"
#include "Message.h"
#include "Database.h"
#include "QueryMessagesThread.h"

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
		throw SQLiteException("Could not open SQLite database", *this);
	}
}

WhatsappDatabase::~WhatsappDatabase()
{
	sqlite3_close(sqLiteDatabase);
}

void WhatsappDatabase::getChats(std::vector<WhatsappChat*> &chats)
{
	const char *query = "SELECT chat_list.key_remote_jid, chat_list.subject, chat_list.creation, max(messages.timestamp) " \
						"FROM chat_list " \
						"LEFT OUTER JOIN messages on messages.key_remote_jid = chat_list.key_remote_jid " \
						"GROUP BY chat_list.key_remote_jid, chat_list.subject, chat_list.creation " \
						"ORDER BY max(messages.timestamp) desc";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(sqLiteDatabase, query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load chat list", *this);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string key = readString(res, 0);
		std::string subject = readString(res, 1);
		long long creation = sqlite3_column_int64(res, 2);
		long long lastMessage = sqlite3_column_int64(res, 3);

		WhatsappChat *chat = new WhatsappChat(*this, key, key, subject, creation, lastMessage);
		chats.push_back(chat);
	}

	sqlite3_finalize(res);
}

void WhatsappDatabase::getMessages(const std::string &chatId, std::vector<WhatsappMessage*> &messages, const volatile bool &running)
{
	QueryMessagesThread queryMessagesThread(*this, sqLiteDatabase, chatId, messages);
	queryMessagesThread.start();

	while (!queryMessagesThread.joinFor(10))
	{
		if (!running)
		{
			queryMessagesThread.interrupt();
			queryMessagesThread.join();
			return;
		}
	}
}

int WhatsappDatabase::getErrorCode()
{
	return sqlite3_errcode(sqLiteDatabase);
}

std::string WhatsappDatabase::getErrorMessage()
{
	return sqlite3_errmsg(sqLiteDatabase);
}
