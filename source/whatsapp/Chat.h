#pragma once

#include <string>

#include "Message.h"

namespace WhatsApp
{

class Chat
{
private:
    std::string id;
    std::vector<Message> messages;

public:
    Chat(const std::string &id);

	std::string getId() const;
	const std::vector<Message> &getMessages() const;
	void setMessages(const std::vector<Message> &messages);
};

}
