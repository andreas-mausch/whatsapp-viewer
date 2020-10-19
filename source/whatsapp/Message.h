#pragma once

#include <optional>
#include <string>

namespace WhatsApp {

class Message {
private:
  std::string id;
  std::string data;
  std::optional<std::string> thumbnail;

public:
  Message(const std::string &id, const std::string &data, const std::optional<std::string> &thumbnail);

  std::string getId() const;
  std::string getData() const;
  std::optional<std::string> getThumbnail() const;
};

} // namespace WhatsApp
