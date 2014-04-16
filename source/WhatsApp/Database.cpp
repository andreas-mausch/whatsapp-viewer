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

		WhatsappChat *chat = new WhatsappChat(*this, key, subject, creation, lastMessage);
		chats.push_back(chat);
	}

	sqlite3_finalize(res);
}

void WhatsappDatabase::getMessages(const std::string &chatId, std::vector<WhatsappMessage*> &messages)
{
	const char *query = "SELECT key_remote_jid, key_from_me, status, data, timestamp, media_url, media_mime_type, media_wa_type, media_size, latitude, longitude, thumb_image, raw_data " \
						"FROM messages " \
						"WHERE key_remote_jid = ? " \
						"ORDER BY timestamp asc";

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
		std::string chatId = readString(res, 0);
		int fromMe = sqlite3_column_int(res, 1);
		int status = sqlite3_column_int(res, 2);
		std::string data = readString(res, 3);
		long long timestamp = sqlite3_column_int64(res, 4);
		std::string mediaUrl = readString(res, 5);
		std::string mediaMimeType = readString(res, 6);
		int mediaWhatsappType = sqlite3_column_int(res, 7);
		int mediaSize = sqlite3_column_int(res, 8);
		double latitude = sqlite3_column_double(res, 9);
		double longitude = sqlite3_column_double(res, 10);
		const void *thumbImage = sqlite3_column_blob(res, 11);
		int thumbImageSize = sqlite3_column_bytes(res, 11);
		const void *rawData = sqlite3_column_blob(res, 12);
		int rawDataSize = sqlite3_column_bytes(res, 12);

		WhatsappMessage *message = new WhatsappMessage(chatId, fromMe == 1, status, data, timestamp, 0, 0, mediaUrl, mediaMimeType, mediaWhatsappType, mediaSize, latitude, longitude, thumbImage, thumbImageSize, rawData, rawDataSize);
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
