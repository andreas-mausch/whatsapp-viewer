#include <windows.h>
#include <sstream>

#include "QueryMessagesThread.h"
#include "Message.h"
#include "../Exceptions/SQLiteException.h"
#include "../Libraries/SQLite/SQLiteDatabase.h"
#include "../Libraries/SQLite/sqlite3.h"

QueryMessagesThread::QueryMessagesThread(WhatsappDatabase& database, SQLiteDatabase& sqLiteDatabase, const std::string& chatId, std::vector<WhatsappMessage*>& messages)
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

WhatsappMessage* QueryMessagesThread::findByMessageId(const std::string& messageId)
{
	for (std::vector<WhatsappMessage*>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		WhatsappMessage* message = *it;
		if (message->getMessageId() == messageId)
		{
			return message;
		}
	}

	return NULL;
}


void QueryMessagesThread::run()
{
	const char* query =
		"SELECT "
		"message.key_id, message.chat_row_id, message.from_me, message.status, message.text_data, message.timestamp, "
		"message_media.message_url, message_media.mime_type, message.message_type, message_media.file_length, "
		"message_media.media_name, message_media.media_caption, message_media.media_duration, "
		"message_location.latitude, message_location.longitude, message_thumbnail.thumbnail, "
		"message_quoted_media.thumbnail, "
		"message_quoted.key_id, message_link._id, "
		"sender_jid.user AS remote_resource "
		"FROM message "
		"LEFT JOIN message_quoted ON message._id = message_quoted.message_row_id "
		"LEFT JOIN message_quoted_media ON message_quoted.message_row_id = message_quoted_media.message_row_id "
		"LEFT JOIN message_link ON message._id = message_link.message_row_id "
		"LEFT JOIN message_media ON message._id = message_media.message_row_id "
		"LEFT JOIN message_location ON message._id = message_location.message_row_id "
		"LEFT JOIN message_thumbnail ON message._id = message_thumbnail.message_row_id "
		"JOIN chat ON chat._id = message.chat_row_id "
		"JOIN jid ON jid._id = chat.jid_row_id "
		"LEFT JOIN jid AS sender_jid ON sender_jid._id = message.sender_jid_row_id "
		"WHERE jid.raw_string = ? "
		"ORDER BY message.timestamp ASC";

	sqlite3_stmt* res;
	if (sqlite3_prepare_v2(sqLiteDatabase.getHandle(), query, -1, &res, NULL) != SQLITE_OK)
	{
		throw SQLiteException("Could not load messages", sqLiteDatabase);
	}

	if (sqlite3_bind_text(res, 1, chatId.c_str(), -1, SQLITE_STATIC) != SQLITE_OK)
	{
		throw SQLiteException("Could not bind sql parameter", sqLiteDatabase);
	}

	int stepResult = sqlite3_step(res);

	while (stepResult == SQLITE_ROW)
	{
		if (!running) break;

		std::string messageId = sqLiteDatabase.readString(res, 0);
		int64_t chatRowId = sqlite3_column_int64(res, 1);
		int fromMe = sqlite3_column_int(res, 2);
		int status = sqlite3_column_int(res, 3);
		std::string data = sqLiteDatabase.readString(res, 4);
		int64_t timestamp = sqlite3_column_int64(res, 5);
		std::string mediaUrl = sqLiteDatabase.readString(res, 6);
		std::string mediaMimeType = sqLiteDatabase.readString(res, 7);
		int mediaWhatsappType = sqlite3_column_int(res, 8);
		int mediaSize = sqlite3_column_int(res, 9);
		std::string mediaName = sqLiteDatabase.readString(res, 10);
		std::string mediaCaption = sqLiteDatabase.readString(res, 11);
		int mediaDuration = sqlite3_column_int(res, 12);
		double latitude = sqlite3_column_type(res, 13) != SQLITE_NULL ? sqlite3_column_double(res, 13) : 0.0;
		double longitude = sqlite3_column_type(res, 14) != SQLITE_NULL ? sqlite3_column_double(res, 14) : 0.0;
		const void* thumbImage = sqlite3_column_blob(res, 15);
		int thumbImageSize = sqlite3_column_bytes(res, 15);
		const void* thumbnailQuoted = sqlite3_column_blob(res, 16);
		int thumbnailQuotedSize = sqlite3_column_bytes(res, 16);
		std::string quotedMessageId = sqLiteDatabase.readString(res, 17);
		int linkId = sqlite3_column_int(res, 18);
		std::string remoteResource = sqLiteDatabase.readString(res, 19);

		WhatsappMessage* quotedMessage = NULL;
		if (!quotedMessageId.empty()) {
			quotedMessage = findByMessageId(quotedMessageId);
		}

		WhatsappMessage* message = new WhatsappMessage(
			messageId, std::to_string(chatRowId), fromMe == 1, status, data, timestamp, 0, 0,
			mediaUrl, mediaMimeType, mediaWhatsappType, mediaSize,
			mediaName, mediaCaption, mediaDuration,
			latitude, longitude, thumbImage, thumbImageSize,
			remoteResource, NULL, 0,
			thumbnailQuoted, thumbnailQuotedSize,
			quotedMessage, linkId > 0
		);
		messages.push_back(message);

		stepResult = sqlite3_step(res);
	}

	sqlite3_finalize(res);
}