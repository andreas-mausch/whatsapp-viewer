#include <windows.h>
#include <sstream>

#include "QueryMessagesThread.h"
#include "Message.h"
#include "../Exceptions/SQLiteException.h"
#include "../Libraries/SQLite/SQLiteDatabase.h"
#include "../Libraries/SQLite/sqlite3.h"

QueryMessagesThread::QueryMessagesThread(WhatsappDatabase &database, SQLiteDatabase &sqLiteDatabase, int chatId, std::vector<WhatsappMessage *> &messages)
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

WhatsappMessage *QueryMessagesThread::findByMessageIdReverse(const std::string &messageId)
{
	for(std::vector<WhatsappMessage *>::reverse_iterator it = messages.rbegin(); it != messages.rend(); ++it)
	{
		WhatsappMessage *message = *it;
		if (message->getMessageId() == messageId)
		{
			return message;
		}
	}

	return NULL;
}

void QueryMessagesThread::run()
{
	const std::string query = "SELECT message_view.key_id, message_view.chat_row_id, message_view.from_me, message_view.starred, message_view.text_data, message_view.timestamp, message_media.message_url, message_media.mime_type, message_view.message_type, message_media.file_length, message_media.media_name, message_view.media_caption, message_media.media_duration, message_location.latitude, message_location.longitude, message_view.thumb_image, message_view.raw_data, message_thumbnail.thumbnail, message_quoted.key_id, message_link._id "
				"FROM message_view "
				"LEFT JOIN message_thumbnail on message_view._id = message_thumbnail.message_row_id "
				"LEFT JOIN message_quoted on message_view._id = message_quoted.message_row_id "
				"LEFT JOIN message_link on message_view._id = message_link.message_row_id "
				"LEFT JOIN message_media on message_view._id = message_media.message_row_id "
				"LEFT JOIN message_location on message_view._id = message_location.message_row_id "
				"WHERE message_view.chat_row_id = ? "
				"ORDER BY message_view.timestamp asc ";

	sqlite3_stmt *res;
	if (sqlite3_prepare_v2(sqLiteDatabase.getHandle(), query.c_str(), -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", sqLiteDatabase);
	}

	if (sqlite3_bind_int(res, 1, chatId) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", sqLiteDatabase);
	}

	while (sqlite3_step(res) == SQLITE_ROW)
	{
		if (!running)
		{
			break;
		}

		std::string messageId = sqLiteDatabase.readString(res, 0);
		std::string chatId = sqLiteDatabase.readString(res, 1);
		int fromMe = sqlite3_column_int(res, 2);
		int status = sqlite3_column_int(res, 3);
		std::string data = sqLiteDatabase.readString(res, 4);
		long long timestamp = sqlite3_column_int64(res, 5);
		std::string mediaUrl = sqLiteDatabase.readString(res, 6);
		std::string mediaMimeType = sqLiteDatabase.readString(res, 7);
		int mediaWhatsappType = sqlite3_column_int(res, 8);
		int mediaSize = sqlite3_column_int(res, 9);
		std::string mediaName = sqLiteDatabase.readString(res, 10);
		std::string mediaCaption = sqLiteDatabase.readString(res, 11);
		int mediaDuration = sqlite3_column_int(res, 12);
		double latitude = sqlite3_column_double(res, 13);
		double longitude = sqlite3_column_double(res, 14);
		const void *thumbImage = sqlite3_column_blob(res, 15);
		int thumbImageSize = sqlite3_column_bytes(res, 15);
		std::string remoteResource = "";
		const void *rawData = sqlite3_column_blob(res, 16);
		int rawDataSize = sqlite3_column_bytes(res, 16);
		const void *thumbnailData = sqlite3_column_blob(res, 17);
		int thumbnailDataSize = sqlite3_column_bytes(res, 17);
		std::string quotedMessageId = sqLiteDatabase.readString(res, 18);
		int linkId = sqlite3_column_int(res, 19);

		WhatsappMessage *quotedMessage = NULL;
		if (quotedMessageId.length() > 0)
		{
			quotedMessage = findByMessageIdReverse(quotedMessageId);
		}
		WhatsappMessage *message = new WhatsappMessage(messageId, chatId, fromMe == 1, status, data, timestamp, 0, 0, mediaUrl, mediaMimeType, mediaWhatsappType, mediaSize, mediaName, mediaCaption, mediaDuration, latitude, longitude, thumbImage, thumbImageSize, remoteResource, rawData, rawDataSize, thumbnailData, thumbnailDataSize, quotedMessage, linkId > 0);
		messages.push_back(message);
	}

	sqlite3_finalize(res);
}
