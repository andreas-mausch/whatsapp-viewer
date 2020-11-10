#pragma once

#include <SQLiteCpp/SQLiteCpp.h>
#include <memory>
#include <vector>

#include "Chat.h"
#include "Message.h"

namespace WhatsApp {

class Database {
private:
  SQLite::Database database;

public:
  Database(const std::string &filename);

  std::vector<std::unique_ptr<Chat>> loadChats();
  std::vector<std::unique_ptr<Message>> loadMessages(const Chat &chat);

  void interrupt();
};

} // namespace WhatsApp
