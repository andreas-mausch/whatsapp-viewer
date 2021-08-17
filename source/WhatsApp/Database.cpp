#include <iostream>
#include <sstream>

#include "../Exceptions/Exception.h"
#include "../Exceptions/KeyNotFoundException.h"
#include "../Exceptions/SQLiteException.h"
#include "../Libraries/SQLite/sqlite3.h"
#include "../Settings.h"
#include "Chat.h"
#include "Message.h"
#include "Database.h"
#include "QueryMessagesThread.h"

WhatsappDatabase::WhatsappDatabase(const std::string &filename)
	: database(filename)
{
	validate();
}

WhatsappDatabase::~WhatsappDatabase()
{
}

void WhatsappDatabase::validate()
{
	if (!hasTable("message_thumbnails")
		|| !hasTable("messages_quotes")
		|| !hasTable("messages_links")
		|| !hasColumn("messages", "quoted_row_id")
		|| !hasColumn("messages", "media_caption"))
	{
		throw Exception("It seems like you tried to open an older WhatsApp database. Please try to use an older version of WhatsApp Viewer.");
	}
}

void WhatsappDatabase::getChats(Settings &settings, std::vector<WhatsappChat*> &chats)
{
	const char *query = "SELECT chat_view.raw_string_jid, chat_view.subject, chat_view.created_timestamp, max(messages.timestamp) " \
						"FROM chat_view " \
						"LEFT OUTER JOIN messages on messages.key_remote_jid = chat_view.raw_string_jid " \
						"WHERE chat_view.hidden = '0' "\
						"GROUP BY chat_view.raw_string_jid, chat_view.subject, chat_view.created_timestamp " \
						"ORDER BY max(messages.timestamp) desc";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(database.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load chat list", database);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string key = database.readString(res, 0);
		std::string subject = database.readString(res, 1);
		long long creation = sqlite3_column_int64(res, 2);
		long long lastMessage = sqlite3_column_int64(res, 3);
		std::string displayName = settings.findDisplayName(key);

		int messagesSent = messagesCount(key, 1);
		int messagesReceived = messagesCount(key, 0);

		WhatsappChat *chat = new WhatsappChat(*this, displayName, key, subject, creation, lastMessage, messagesSent, messagesReceived);
		chats.push_back(chat);
	}

	sqlite3_finalize(res);
}

int WhatsappDatabase::messagesCount(const std::string &chatId, int fromMe)
{
	const char *query = "SELECT count(_id) from messages where key_remote_jid = ? and key_from_me = ?";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(database.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", database);
	}

	if (sqlite3_bind_text(res, 1, chatId.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", database);
	}

	if (sqlite3_bind_int(res, 2, fromMe) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", database);
	}

	if (sqlite3_step(res) != SQLITE_ROW)
	{
		throw SQLiteException("No result for count query", database);
	}

	int count = sqlite3_column_int(res, 0);
	sqlite3_finalize(res);

	return count;
}

void WhatsappDatabase::getMessages(const std::string &chatId, std::vector<WhatsappMessage*> &messages, const volatile bool &running)
{
	QueryMessagesThread queryMessagesThread(*this, database, chatId, messages);
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


bool WhatsappDatabase::hasTable(const std::string &tableName)
{
	const char *query = "SELECT name FROM sqlite_master WHERE type='table' AND name = ?";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(database.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not query tables", database);
	}
	if (sqlite3_bind_text(res, 1, tableName.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", database);
	}

	bool hasTable = false;
	if (sqlite3_step(res) == SQLITE_ROW)
	{
		hasTable = true;
	}

	sqlite3_finalize(res);
	return hasTable;
}

bool WhatsappDatabase::hasColumn(const std::string &tableName, const std::string &columnName)
{
	std::stringstream query;
	query << "PRAGMA table_info('" << tableName << "')";
	std::string queryString = query.str();

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(database.getHandle(), queryString.c_str(), -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not query columns", database);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string columnNameDb = database.readString(res, 1);

		if (columnName == columnNameDb)
		{
			sqlite3_finalize(res);
			return true;
		}
	}

	sqlite3_finalize(res);
	return false;
}
