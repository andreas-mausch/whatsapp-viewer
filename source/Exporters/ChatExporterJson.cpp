#include <fstream>
#include <sstream>
#include <windows.h>

#include "ChatExporterJson.h"
#include "../Exceptions/Exception.h"
#include "../Exceptions/KeyNotFoundException.h"
#include "../WhatsApp/Chat.h"
#include "../WhatsApp/Message.h"
#include "../Platforms/Win32/Timestamp.h"
#include "../Libraries/Base64/Base64.h"
#include "../Settings.h"

ChatExporterJson::ChatExporterJson(Settings &settings)
	: settings(settings)
{
}

ChatExporterJson::~ChatExporterJson()
{
}

void ChatExporterJson::addImageParameter(WhatsappMessage &message, rapidjson::Value &messageJson, rapidjson::Document &json)
{
	addImageParameter(message.getThumbnail(), message.getThumbnailSize(), messageJson, json);
}

void ChatExporterJson::addImageParameter(unsigned char *image, int size, rapidjson::Value &messageJson, rapidjson::Document &json)
{
	if (image != NULL && size > 0)
	{
		messageJson.AddMember("image", base64_encode(image, size), json.GetAllocator());
	}
}

void ChatExporterJson::addFilename(WhatsappMessage &message, rapidjson::Value &messageJson, rapidjson::Document &json)
{
	if (message.getFilename().length() > 0)
	{
		messageJson.AddMember("filename", message.getFilename(), json.GetAllocator());
	}
}

void ChatExporterJson::exportChat(WhatsappChat &chat, rapidjson::Value &chatJson, rapidjson::Document &json)
{
	chatJson.AddMember("key", chat.getKey(), json.GetAllocator());

	if (chat.getSubject().length() > 0)
	{
		chatJson.AddMember("subject", chat.getSubject(), json.GetAllocator());
	}

	chatJson.AddMember("contactName", chat.getDisplayName(), json.GetAllocator());

	rapidjson::Value messagesJson(rapidjson::kArrayType);

	std::vector<WhatsappMessage *> &messages = chat.getMessages(true);
	for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		WhatsappMessage &message = **it;
		rapidjson::Value messageJson(rapidjson::kObjectType);

		messageJson.AddMember("timestamp", formatTimestampIso(message.getTimestamp()), json.GetAllocator());
		messageJson.AddMember("id", message.getMessageId(), json.GetAllocator());
		messageJson.AddMember("fromMe", message.isFromMe(), json.GetAllocator());

		if (message.getRemoteResource().size() > 0)
		{
			messageJson.AddMember("remoteResource", message.getRemoteResource(), json.GetAllocator());
			messageJson.AddMember("remoteResourceDisplayName", settings.findDisplayName(message.getRemoteResource()), json.GetAllocator());
		}

		if (message.getQuotedMessage() != NULL)
		{
			messageJson.AddMember("quotedTimestamp", formatTimestampIso(message.getQuotedMessage()->getTimestamp()), json.GetAllocator());
			messageJson.AddMember("quotedMessageId", message.getQuotedMessage()->getMessageId(), json.GetAllocator());
		}

		switch (message.getMediaWhatsappType())
		{
			case MEDIA_WHATSAPP_TEXT:
			{
				if (message.isLink())
				{
					messageJson.AddMember("type", "link", json.GetAllocator());
					if (message.getMediaCaption().length() > 0)
					{
						messageJson.AddMember("caption", message.getMediaCaption(), json.GetAllocator());
					}
					addImageParameter(message.getLinkThumbnail(), message.getLinkThumbnailSize(), messageJson, json);
					messageJson.AddMember("link", message.getData(), json.GetAllocator());
				}
				else
				{
					messageJson.AddMember("type", "text", json.GetAllocator());
					messageJson.AddMember("text", message.getData(), json.GetAllocator());
				}
			} break;
			case MEDIA_WHATSAPP_IMAGE:
			case MEDIA_WHATSAPP_GIF:
			{
				messageJson.AddMember("type", "image", json.GetAllocator());
				addImageParameter(message, messageJson, json);
				addFilename(message, messageJson, json);

				if (message.getMediaCaption().length() > 0)
				{
					messageJson.AddMember("caption", message.getMediaCaption(), json.GetAllocator());
				}
			} break;
			case MEDIA_WHATSAPP_AUDIO:
			{
				messageJson.AddMember("type", "audio", json.GetAllocator());
				messageJson.AddMember("duration", message.getMediaDuration(), json.GetAllocator());
				addFilename(message, messageJson, json);
			} break;
			case MEDIA_WHATSAPP_VIDEO:
			{
				messageJson.AddMember("type", "video", json.GetAllocator());
				addImageParameter(message, messageJson, json);
				addFilename(message, messageJson, json);
			} break;
			case MEDIA_WHATSAPP_CONTACT:
			{
				messageJson.AddMember("type", "contact", json.GetAllocator());
			} break;
			case MEDIA_WHATSAPP_LOCATION:
			case MEDIA_WHATSAPP_LIVE_LOCATION:
			{
				messageJson.AddMember("type", "location", json.GetAllocator());

				std::stringstream location;
				location << message.getLatitude() << "," << message.getLongitude();
				messageJson.AddMember("location", location.str(), json.GetAllocator());

				addImageParameter(message, messageJson, json);
			} break;
		}

		messagesJson.PushBack(messageJson, json.GetAllocator());
	}

	chatJson.AddMember("messages", messagesJson, json.GetAllocator());

}

void ChatExporterJson::exportChats(const std::vector<WhatsappChat *> &chats, const std::string &filename)
{
	rapidjson::Document json;
	json.SetObject();

	rapidjson::Value chatsJson(rapidjson::kArrayType);

	for (auto i = chats.begin(); i != chats.end(); ++i) {
		WhatsappChat *chat = *i;

		rapidjson::Value chatJson(rapidjson::kObjectType);
		exportChat(*chat, chatJson, json);
		chatsJson.PushBack(chatJson, json.GetAllocator());
	}

	json.AddMember("chats", chatsJson, json.GetAllocator());

	rapidjson::StringBuffer buffer;
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
	json.Accept(writer);

	std::ofstream file(filename.c_str());
	if (!file)
	{
		throw Exception("could not open chat export file");
	}

	file << buffer.GetString();
}
