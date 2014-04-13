#include <iostream>
#include <sstream>

#include "../Exceptions/Exception.h"
#include "../Exceptions/SQLiteException.h"
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
		throw SQLiteException("Could not open SQLite database", *this);
	}
}

WhatsappDatabase::~WhatsappDatabase()
{
	sqlite3_close(sqLiteDatabase);
}

void WhatsappDatabase::getChats(std::vector<WhatsappChat*> &chats)
{
	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(sqLiteDatabase, "SELECT * FROM chat_list", -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load chat list", *this);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string key = readString(res, 1);
		std::string subject = readString(res, 3);
		long long creation = sqlite3_column_int64(res, 4);

		WhatsappChat *chat = new WhatsappChat(*this, key, subject, creation);
		chats.push_back(chat);
	}

	sqlite3_finalize(res);
}

void WhatsappDatabase::getMessages(const std::string &chatId, std::vector<WhatsappMessage*> &messages)
{
	const char *query = "SELECT * FROM messages where key_remote_jid = ? order by timestamp asc";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(sqLiteDatabase, query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", *this);
	}

	if (sqlite3_bind_text(res, 1, chatId.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", *this);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		std::string chatId = readString(res, 1);
		int fromMe = sqlite3_column_int(res, 2);
		int status = sqlite3_column_int(res, 3);
		std::string data = readString(res, 6);
		long long timestamp = sqlite3_column_int64(res, 7);
		std::string mediaUrl = readString(res, 8);
		std::string mediaMimeType = readString(res, 9);
		int mediaWhatsappType = sqlite3_column_int(res, 10);
		int mediaSize = sqlite3_column_int(res, 11);
		const void *thumbImage = sqlite3_column_blob(res, 18);
		int thumbImageSize = sqlite3_column_bytes(res, 18);
		const void *rawData = sqlite3_column_blob(res, 24);
		int rawDataSize = sqlite3_column_bytes(res, 24);

		WhatsappMessage *message = new WhatsappMessage(chatId, fromMe == 1, status, data, timestamp, 0, 0, mediaUrl, mediaMimeType, mediaWhatsappType, mediaSize, thumbImage, thumbImageSize, rawData, rawDataSize);
		messages.push_back(message);
	}

	sqlite3_finalize(res);
}

int WhatsappDatabase::getErrorCode()
{
	return sqlite3_errcode(sqLiteDatabase);
}

std::string WhatsappDatabase::getErrorMessage()
{
	return sqlite3_errmsg(sqLiteDatabase);
}
