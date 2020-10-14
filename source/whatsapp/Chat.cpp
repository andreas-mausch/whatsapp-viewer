#include "Database.h"

namespace WhatsApp
{

Chat::Chat(const std::string &id)
    : id(id)
{
}

std::string Chat::getId() const
{
    return id;
}

const std::vector<Message> &Chat::getMessages() const
{
    return messages;
}

void Chat::setMessages(const std::vector<Message> &messages)
{
    this->messages = messages;
}

}
