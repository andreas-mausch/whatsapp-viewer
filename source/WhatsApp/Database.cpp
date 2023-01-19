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
	if (!hasTable("message_thumbnail")
		|| !hasTable("message_quoted")
		|| !hasTable("message_link")
		|| !hasColumn("message_quoted", "chat_row_id")
		|| !hasColumn("message_view", "media_caption"))
	{
		throw Exception("It seems like you tried to open an older WhatsApp database. Please try to use an older version of WhatsApp Viewer.");
	}
}

void WhatsappDatabase::getChats(Settings &settings, std::vector<WhatsappChat*> &chats)
{
	const std::string query = "SELECT chat_view.raw_string_jid, chat_view.subject, chat_view.created_timestamp, max(message.timestamp), chat_view._id "
						"FROM chat_view "
						"LEFT OUTER JOIN message on message.chat_row_id = chat_view._id "
						"WHERE chat_view.hidden = 0 "
						"GROUP BY chat_view.raw_string_jid, chat_view.subject, chat_view.created_timestamp "
						"ORDER BY max(message.timestamp) desc";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(database.getHandle(), query.c_str(), -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load chat list", database);
	}

	const std::string count_query = "SELECT count(_id) from message_view where chat_row_id = ? and from_me = ?";
	sqlite3_stmt* count_res;

	if (sqlite3_prepare_v2(database.getHandle(), count_query.c_str(), -1, &count_res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", database);
	}


	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string key = database.readString(res, 0);
		std::string subject = database.readString(res, 1);
		long long creation = sqlite3_column_int64(res, 2);
		long long lastMessage = sqlite3_column_int64(res, 3);
		std::string displayName = settings.findDisplayName(key);
		int chat_id = sqlite3_column_int64(res, 4);

		int messagesSent = messagesCount(chat_id, 1, count_res);
		int messagesReceived = messagesCount(chat_id, 0, count_res);

		WhatsappChat *chat = new WhatsappChat(*this, chat_id,displayName, key, subject, creation, lastMessage, messagesSent, messagesReceived);
		chats.push_back(chat);
	}

	sqlite3_finalize(res);
}

int WhatsappDatabase::messagesCount(int chatId, int fromMe, sqlite3_stmt *res)
{
	if (sqlite3_bind_int(res, 1, chatId) != SQLITE_OK)
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
	sqlite3_reset(res);

	return count;
}

void WhatsappDatabase::getMessages(int chatId, std::vector<WhatsappMessage*> &messages, const volatile bool &running)
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
