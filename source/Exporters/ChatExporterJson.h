#pragma once

#include <string>

#include "ChatExporter.h"

class WhatsappChat;

class ChatExporterJson : public ChatExporter
{
public:
	ChatExporterJson();
	~ChatExporterJson();

	void exportChat(WhatsappChat &chat, const std::string &filename);
};
