#pragma once

#include <string>

class WhatsappChat;

class ChatExporterTxt
{
private:
	WhatsappChat &chat;

public:
	ChatExporterTxt(WhatsappChat &chat);
	~ChatExporterTxt();

	void exportChat(const std::string &filename);
};
