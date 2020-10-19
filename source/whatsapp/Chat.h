#pragma once

#include <memory>
#include <string>

#include "Message.h"

namespace WhatsApp {

class Chat {
private:
  std::string id;
  std::vector<std::unique_ptr<Message>> messages;

public:
  Chat(const std::string &id);

  std::string getId() const;
  const std::vector<std::unique_ptr<Message>> &getMessages() const;
  void setMessages(const std::vector<std::unique_ptr<Message>> messages);
};

} // namespace WhatsApp
