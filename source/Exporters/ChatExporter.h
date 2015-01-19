#pragma once

#include <string>

class WhatsappChat;

class ChatExporter
{
public:
	ChatExporter() {}
	virtual ~ChatExporter() {}

	virtual void exportChat(WhatsappChat &chat, const std::string &filename) = 0;
};
