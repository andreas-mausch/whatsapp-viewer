#pragma once

#include <string>

#include "ChatExporter.h"

class WhatsappChat;

class ChatExporterTxt : public ChatExporter
{
private:
	WhatsappChat &chat;

public:
	ChatExporterTxt(WhatsappChat &chat);
	~ChatExporterTxt();

	void exportChat(const std::string &filename);
};
