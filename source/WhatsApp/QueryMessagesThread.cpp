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
	const char *query = "SELECT key_remote_jid, key_from_me, status, data, timestamp, media_url, media_mime_type, media_wa_type, media_size, media_name, media_duration, latitude, longitude, thumb_image, remote_resource, raw_data " \
						"FROM messages " \
						"WHERE key_remote_jid = ? " \
						"ORDER BY timestamp asc";

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

		WhatsappMessage *message = new WhatsappMessage(chatId, fromMe == 1, status, data, timestamp, 0, 0, mediaUrl, mediaMimeType, mediaWhatsappType, mediaSize, mediaName, mediaDuration, latitude, longitude, thumbImage, thumbImageSize, remoteResource, rawData, rawDataSize);
		messages.push_back(message);
	}

	sqlite3_finalize(res);
}
