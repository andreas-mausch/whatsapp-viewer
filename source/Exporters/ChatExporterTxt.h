#pragma once

#include <string>

#include "ChatExporter.h"

class WhatsappChat;

class ChatExporterTxt : public ChatExporter
{
public:
	ChatExporterTxt();
	~ChatExporterTxt();

	void exportChat(WhatsappChat &chat, const std::string &filename);
};
