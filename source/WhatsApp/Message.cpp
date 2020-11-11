#include "Database.h"

namespace WhatsApp {

Message::Message(const std::string &id, const std::string &data,
                 const std::optional<std::string> &thumbnail)
    : id(id), data(data), thumbnail(thumbnail) {}

std::string Message::getId() const { return id; }

std::string Message::getData() const { return data; }

std::optional<std::string> Message::getThumbnail() const { return thumbnail; }

} // namespace WhatsApp
