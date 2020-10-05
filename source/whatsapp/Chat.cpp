#include "Database.h"

namespace WhatsApp
{

Chat::Chat(const std::string &id)
    : id(id)
{
}

std::string Chat::getId()
{
    return id;
}

}
