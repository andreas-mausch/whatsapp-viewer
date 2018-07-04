#pragma once

#include <string>

#include "ChatExporter.h"

#define RAPIDJSON_HAS_STDSTRING 1

#include "../Libraries/Json/rapidjson/document.h"
#include "../Libraries/Json/rapidjson/prettywriter.h"
#include "../Libraries/Json/rapidjson/stringbuffer.h"

class Settings;
class WhatsappChat;
class WhatsappMessage;

class ChatExporterJson : public ChatExporter
{
private:
	Settings &settings;

	void addImageParameter(WhatsappMessage &message, rapidjson::Value &messageJson, rapidjson::Document &json);
	void addImageParameter(unsigned char *image, int size, rapidjson::Value &messageJson, rapidjson::Document &json);
	void addFilename(WhatsappMessage &message, rapidjson::Value &messageJson, rapidjson::Document &json);
	std::string findDisplayName(const std::string &key);
	void exportChat(WhatsappChat &chat, rapidjson::Value &chatJson, rapidjson::Document &json);

public:
	ChatExporterJson(Settings &settings);
	~ChatExporterJson();

	void exportChats(const std::vector<WhatsappChat *> &chats, const std::string &filename);
};
