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
	if (message.hasThumbnail())
	{
		messageJson.AddMember("image", base64_encode(message.getThumbnail(), message.getThumbnailSize()), json.GetAllocator());
	}
}

void ChatExporterJson::exportChat(WhatsappChat &chat, const std::string &filename)
{
	rapidjson::Document json;
	json.SetObject();

	json.AddMember("key", chat.getKey(), json.GetAllocator());

	if (chat.getSubject().length() > 0)
	{
		json.AddMember("subject", chat.getSubject(), json.GetAllocator());
	}

	json.AddMember("contactName", chat.getDisplayName(), json.GetAllocator());

	rapidjson::Value messagesJson(rapidjson::kArrayType);

	std::vector<WhatsappMessage *> &messages = chat.getMessages(true);
	for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		WhatsappMessage &message = **it;
		rapidjson::Value messageJson(rapidjson::kObjectType);

		messageJson.AddMember("timestamp", formatTimestampIso(message.getTimestamp()), json.GetAllocator());
		messageJson.AddMember("fromMe", message.isFromMe(), json.GetAllocator());

		if (message.getRemoteResource().size() > 0)
		{
			messageJson.AddMember("remoteResource", message.getRemoteResource(), json.GetAllocator());
			messageJson.AddMember("remoteResourceDisplayName", settings.findDisplayName(message.getRemoteResource()), json.GetAllocator());
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
					if (message.hasLinkThumbnail())
					{
						messageJson.AddMember("image", base64_encode(message.getLinkThumbnail(), message.getLinkThumbnailSize()), json.GetAllocator());
					}
					messageJson.AddMember("link", message.getData(), json.GetAllocator());
				}
				else
				{
					messageJson.AddMember("type", "text", json.GetAllocator());
					messageJson.AddMember("text", message.getData(), json.GetAllocator());
				}
			} break;
			case MEDIA_WHATSAPP_IMAGE:
			{
				messageJson.AddMember("type", "image", json.GetAllocator());
				addImageParameter(message, messageJson, json);

				if (message.getMediaCaption().length() > 0)
				{
					messageJson.AddMember("caption", message.getMediaCaption(), json.GetAllocator());
				}
			} break;
			case MEDIA_WHATSAPP_AUDIO:
			{
				messageJson.AddMember("type", "audio", json.GetAllocator());
			} break;
			case MEDIA_WHATSAPP_VIDEO:
			{
				messageJson.AddMember("type", "video", json.GetAllocator());
				addImageParameter(message, messageJson, json);
			} break;
			case MEDIA_WHATSAPP_CONTACT:
			{
				messageJson.AddMember("type", "contact", json.GetAllocator());
			} break;
			case MEDIA_WHATSAPP_LOCATION:
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

	json.AddMember("messages", messagesJson, json.GetAllocator());

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
