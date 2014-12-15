#pragma once

#include <string>

class WhatsappChat;

class ChatExporterHtml
{
private:
	WhatsappChat &chat;
	std::string templateHtml;

	std::string buildMessages();

public:
	ChatExporterHtml(const std::string &templateHtml, WhatsappChat &chat);
	~ChatExporterHtml();

	void exportChat(const std::string &filename);
};
