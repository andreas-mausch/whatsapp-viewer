#pragma once

#include <string>

class WhatsappChat;

class ChatExporter
{
private:
	WhatsappChat &chat;

public:
	ChatExporter(WhatsappChat &chat);
	~ChatExporter();

	void exportChat(const std::string &filename);
};
