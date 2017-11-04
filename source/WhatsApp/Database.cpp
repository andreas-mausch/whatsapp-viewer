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
}

WhatsappDatabase::~WhatsappDatabase()
{
}

void WhatsappDatabase::getChats(Settings &settings, std::vector<WhatsappChat*> &chats)
{
	const char *query = "SELECT chat_list.key_remote_jid, chat_list.subject, chat_list.creation, max(messages.timestamp) " \
						"FROM chat_list " \
						"LEFT OUTER JOIN messages on messages.key_remote_jid = chat_list.key_remote_jid " \
						"GROUP BY chat_list.key_remote_jid, chat_list.subject, chat_list.creation " \
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

