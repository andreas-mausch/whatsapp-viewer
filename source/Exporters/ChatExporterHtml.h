#pragma once

#include <string>
#include <set>

#include "ChatExporter.h"

class WhatsappChat;
class WhatsappMessage;

class ChatExporterHtml : public ChatExporter
{
private:
	WhatsappChat &chat;
	std::string templateHtml;

	std::string buildMessages(std::set<int> &usedEmoticons);
	void replacePlaceholder(std::string &html, const std::string &placeholder, const std::string &text);
	std::string convertMessageToHtml(WhatsappMessage &message, std::set<int> &usedEmoticons);
	std::string buildEmoticonStyles(const std::set<int> &usedEmoticons);

public:
	ChatExporterHtml(const std::string &templateHtml, WhatsappChat &chat);
	~ChatExporterHtml();

	void exportChat(const std::string &filename);
};
