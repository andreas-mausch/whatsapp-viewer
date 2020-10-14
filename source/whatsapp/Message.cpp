#include "Database.h"

namespace WhatsApp
{

Message::Message(const std::string &id)
    : id(id)
{
}

std::string Message::getId() const
{
    return id;
}

}
