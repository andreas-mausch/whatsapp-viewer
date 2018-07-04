#pragma once

#include <string>
#include <vector>

class WhatsappChat;

class ChatExporter
{
public:
	ChatExporter() {}
	virtual ~ChatExporter() {}

	virtual void exportChats(const std::vector<WhatsappChat *> &chats, const std::string &filename) = 0;
};
