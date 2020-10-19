#include "Database.h"

namespace WhatsApp {

Database::Database(const std::string &filename) : database(filename) {}

std::vector<std::unique_ptr<Chat>> Database::loadChats() {
  std::string query = "SELECT chat_list.key_remote_jid, chat_list.subject, "
                      "chat_list.creation, max(messages.timestamp) "
                      "FROM chat_list "
                      "LEFT OUTER JOIN messages on messages.key_remote_jid = "
                      "chat_list.key_remote_jid "
                      "GROUP BY chat_list.key_remote_jid, chat_list.subject, "
                      "chat_list.creation "
                      "ORDER BY max(messages.timestamp) desc";

  SQLite::Statement statement(database, query);
  std::vector<std::unique_ptr<Chat>> chats;
  while (statement.executeStep()) {
    std::string id = statement.getColumn(0);
    chats.push_back(std::make_unique<Chat>(id));
  }

  return chats;
}

std::vector<std::unique_ptr<Message>> Database::loadMessages(const Chat &chat) {
  std::string query =
      "SELECT messages.key_id, messages.key_remote_jid, messages.key_from_me, "
      "messages.status, messages.data, messages.timestamp, messages.media_url, "
      "messages.media_mime_type, messages.media_wa_type, messages.media_size, "
      "messages.media_name, messages.media_caption, messages.media_duration, "
      "messages.latitude, messages.longitude, messages.thumb_image, "
      "messages.remote_resource, messages.raw_data, "
      "message_thumbnails.thumbnail, messages_quotes.key_id, "
      "messages_links._id "
      "FROM messages "
      "LEFT JOIN message_thumbnails on messages.key_id = "
      "message_thumbnails.key_id "
      "LEFT JOIN messages_quotes on messages.quoted_row_id > 0 AND "
      "messages.quoted_row_id = messages_quotes._id "
      "LEFT JOIN messages_links on messages._id = "
      "messages_links.message_row_id "
      "WHERE messages.key_remote_jid = ? "
      "ORDER BY messages.timestamp asc";

  SQLite::Statement statement(database, query);
  statement.bind(1, chat.getId());

  std::vector<std::unique_ptr<Message>> messages;
  while (statement.executeStep()) {
    std::string id = statement.getColumn(0);
    std::string data = statement.getColumn(4);
    std::string thumbnail = statement.getColumn(18);
    messages.push_back(std::make_unique<Message>(id, data, thumbnail.length() > 0 ? std::make_optional(thumbnail) : std::nullopt));
  }

  return messages;
}

} // namespace WhatsApp
