#pragma once

#include <string>

class WhatsappChat;
class WhatsappMessage;

class ChatExporterHtml
{
private:
	WhatsappChat &chat;
	std::string templateHtml;

	std::string buildMessages();
	void replacePlaceholder(std::string &html, const std::string &placeholder, const std::string &text);
	std::string convertMessageToHtml(WhatsappMessage &message);

public:
	ChatExporterHtml(const std::string &templateHtml, WhatsappChat &chat);
	~ChatExporterHtml();

	void exportChat(const std::string &filename);
};
