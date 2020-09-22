
#pragma once

#include <string>
#include <set>

#include "ChatExporter.h"

class Settings;
class WhatsappChat;
class WhatsappMessage;

class ChatExporterHtml : public ChatExporter
{
private:
	Settings &settings;
	std::string templateHtml;

	std::string buildMessages(WhatsappChat &chat, std::set<int> &usedEmoticons);
	void replacePlaceholder(std::string &html, const std::string &placeholder, const std::string &text);
	std::string convertMessageToHtml(std::string message, std::set<int> &usedEmoticons);
	std::string buildEmoticonStyles(const std::set<int> &usedEmoticons);

public:
	ChatExporterHtml(Settings &settings, const std::string &templateHtml);
	~ChatExporterHtml();

	void exportChat(WhatsappChat &chat, const std::string &filename);
};
