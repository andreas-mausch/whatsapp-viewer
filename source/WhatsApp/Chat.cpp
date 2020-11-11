#include "Database.h"

namespace WhatsApp {

Chat::Chat(const std::string &id) : id(id) {}

std::string Chat::getId() const { return id; }

const std::vector<std::unique_ptr<Message>> &Chat::getMessages() const {
  return messages;
}

void Chat::setMessages(std::vector<std::unique_ptr<Message>> messages) {
  this->messages = std::move(messages);
}

} // namespace WhatsApp
