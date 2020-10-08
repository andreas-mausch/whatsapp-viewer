#include "Database.h"

namespace WhatsApp
{

Database::Database(const std::string &filename)
    : database(filename)
{
}

std::vector<Chat> Database::loadChats()
{
    std::string query = "SELECT chat_list.key_remote_jid, chat_list.subject, chat_list.creation, max(messages.timestamp) " \
                "FROM chat_list " \
                "LEFT OUTER JOIN messages on messages.key_remote_jid = chat_list.key_remote_jid " \
                "GROUP BY chat_list.key_remote_jid, chat_list.subject, chat_list.creation " \
                "ORDER BY max(messages.timestamp) desc";

    SQLite::Statement statement(database, query);
    std::vector<Chat> chats;
    while (statement.executeStep())
    {
        std::string id = statement.getColumn(0);
        chats.push_back(Chat(id));
    }

    return chats;
}

}
