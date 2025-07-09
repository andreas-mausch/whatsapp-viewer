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
		|| !hasTable("message_link")
		|| !hasColumn("message_quoted", "message_row_id")
		|| !hasColumn("message_media", "media_caption"))
	{
		throw Exception("It seems like you tried to open an older WhatsApp database. Please try to use an older version of WhatsApp Viewer.");
	}
}

void WhatsappDatabase::getChats(Settings& settings, std::vector<WhatsappChat*>& chats)
{
	const char* query =
		"SELECT j.raw_string, c.subject, c.created_timestamp, MAX(m.timestamp) "
		"FROM chat c "
		"JOIN jid j ON j._id = c.jid_row_id "
		"LEFT JOIN message m ON m.chat_row_id = c._id "
		"WHERE c.hidden = 0 "
		"GROUP BY j.raw_string, c.subject, c.created_timestamp "
		"ORDER BY MAX(m.timestamp) DESC";

	sqlite3_stmt* res;
	if (sqlite3_prepare_v2(database.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load chat list", database);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string key = database.readString(res, 0);            // j.raw_string
		std::string subject = database.readString(res, 1);        // c.subject
		long long creation = sqlite3_column_int64(res, 2);        // c.created_timestamp
		long long lastMessage = sqlite3_column_int64(res, 3);     // MAX(m.timestamp)

		std::string displayName = settings.findDisplayName(key);  // Custom display

		int messagesSent = messagesCount(key, 1);     // from_me = 1
		int messagesReceived = messagesCount(key, 0); // from_me = 0

		WhatsappChat* chat = new WhatsappChat(*this, displayName, key, subject, creation, lastMessage, messagesSent, messagesReceived);
		chats.push_back(chat);
	}

	sqlite3_finalize(res);
}

int WhatsappDatabase::messagesCount(const std::string& chatId, int fromMe)
{
	const char* query =
		"SELECT count(m._id) "
		"FROM message m "
		"JOIN chat c ON c._id = m.chat_row_id "
		"JOIN jid j ON j._id = c.jid_row_id "
		"WHERE j.raw_string = ? AND m.from_me = ?";

	sqlite3_stmt* res;
	if (sqlite3_prepare_v2(database.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", database);
	}

	if (sqlite3_bind_text(res, 1, chatId.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind chatId parameter", database);
	}

	if (sqlite3_bind_int(res, 2, fromMe) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind fromMe parameter", database);
	}

	int count = 0;
	if (sqlite3_step(res) == SQLITE_ROW)
	{
		count = sqlite3_column_int(res, 0);
	}
	else
	{
		throw SQLiteException("No result for count query", database);
	}

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
