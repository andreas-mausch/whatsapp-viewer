#pragma once

#include <fstream>
#include <string>

#include "ChatExporter.h"

class WhatsappChat;

class ChatExporterTxt : public ChatExporter
{
private:
	void exportChat(WhatsappChat &chat, std::ofstream &file);

public:
	ChatExporterTxt();
	~ChatExporterTxt();

	void exportChats(const std::vector<WhatsappChat *> &chats, const std::string &filename);
};
