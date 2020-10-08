#pragma once

#include <vector>
#include <SQLiteCpp/SQLiteCpp.h>

#include "Chat.h"

namespace WhatsApp
{

class Database
{
private:
    SQLite::Database database;

public:
    Database(const std::string &filename);

	std::vector<Chat> loadChats();
};

}
