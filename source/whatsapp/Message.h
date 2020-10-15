#pragma once

#include <string>

namespace WhatsApp {

class Message {
private:
  std::string id;

public:
  Message(const std::string &id);

  std::string getId() const;
};

} // namespace WhatsApp
