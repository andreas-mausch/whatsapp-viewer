#include <windows.h>

#include "QueryMessagesThread.h"
#include "Message.h"
#include "../Exceptions/SQLiteException.h"
#include "../Libraries/SQLite/SQLiteDatabase.h"
#include "../Libraries/SQLite/sqlite3.h"

QueryMessagesThread::QueryMessagesThread(WhatsappDatabase &database, SQLiteDatabase &sqLiteDatabase, const std::string &chatId, std::vector<WhatsappMessage *> &messages)
	: database(database), sqLiteDatabase(sqLiteDatabase), chatId(chatId), messages(messages)
{
}

QueryMessagesThread::~QueryMessagesThread()
{
}

void QueryMessagesThread::interrupt()
{
	ThreadWindows::interrupt();
	sqlite3_interrupt(sqLiteDatabase.getHandle());
}

void QueryMessagesThread::run()
{
	const char *query = "SELECT messages.key_remote_jid, messages.key_from_me, status, messages.data, messages.timestamp, messages.media_url, messages.media_mime_type, messages.media_wa_type, messages.media_size, messages.media_name, messages.media_duration, messages.latitude, messages.longitude, messages.thumb_image, messages.remote_resource, messages.raw_data, message_thumbnails.thumbnail " \
						"FROM messages " \
						"LEFT JOIN message_thumbnails on messages.key_id = message_thumbnails.key_id " \
						"WHERE messages.key_remote_jid = ? " \
						"ORDER BY messages.timestamp asc";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(sqLiteDatabase.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", sqLiteDatabase);
	}

	if (sqlite3_bind_text(res, 1, chatId.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", sqLiteDatabase);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		if (!running)
		{
			break;
		}

		std::string chatId = sqLiteDatabase.readString(res, 0);
		int fromMe = sqlite3_column_int(res, 1);
		int status = sqlite3_column_int(res, 2);
		std::string data = sqLiteDatabase.readString(res, 3);
		long long timestamp = sqlite3_column_int64(res, 4);
		std::string mediaUrl = sqLiteDatabase.readString(res, 5);
		std::string mediaMimeType = sqLiteDatabase.readString(res, 6);
		int mediaWhatsappType = sqlite3_column_int(res, 7);
		int mediaSize = sqlite3_column_int(res, 8);
		std::string mediaName = sqLiteDatabase.readString(res, 9);
		int mediaDuration = sqlite3_column_int(res, 10);
		double latitude = sqlite3_column_double(res, 11);
		double longitude = sqlite3_column_double(res, 12);
		const void *thumbImage = sqlite3_column_blob(res, 13);
		int thumbImageSize = sqlite3_column_bytes(res, 13);
		std::string remoteResource = sqLiteDatabase.readString(res, 14);
		const void *rawData = sqlite3_column_blob(res, 15);
		int rawDataSize = sqlite3_column_bytes(res, 15);
		const void *thumbnailData = sqlite3_column_blob(res, 16);
		int thumbnailDataSize = sqlite3_column_bytes(res, 16);

		WhatsappMessage *message = new WhatsappMessage(chatId, fromMe == 1, status, data, timestamp, 0, 0, mediaUrl, mediaMimeType, mediaWhatsappType, mediaSize, mediaName, mediaDuration, latitude, longitude, thumbImage, thumbImageSize, remoteResource, rawData, rawDataSize, thumbnailData, thumbnailDataSize);
		messages.push_back(message);
	}

	sqlite3_finalize(res);
}
