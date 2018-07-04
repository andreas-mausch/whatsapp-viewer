#include <fstream>
#include <windows.h>

#include "ChatExporterTxt.h"
#include "../Exceptions/Exception.h"
#include "../WhatsApp/Chat.h"
#include "../WhatsApp/Message.h"
#include "../Platforms/Win32/Timestamp.h"

ChatExporterTxt::ChatExporterTxt()
{
}

ChatExporterTxt::~ChatExporterTxt()
{
}

void ChatExporterTxt::exportChat(WhatsappChat &chat, std::ofstream &file)
{
	file << chat.getKey();

	if (chat.getSubject().length() > 0)
	{
		file << "; " << chat.getSubject();
	}

	file << std::endl << std::endl;

	std::vector<WhatsappMessage *> &messages = chat.getMessages(true);
	for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		WhatsappMessage &message = **it;

		file << formatTimestamp(message.getTimestamp()) << "; ";

		if (message.isFromMe())
		{
			file << "ME: ";
		}
		else
		{
			file << "he: ";
		}

		if (message.getQuotedMessage() != NULL)
		{
			file << "[ Quote from " << formatTimestamp(message.getQuotedMessage()->getTimestamp()) << ": " << message.getQuotedMessage()->getMessageId() << " ]; ";
		}

		switch (message.getMediaWhatsappType())
		{
			case MEDIA_WHATSAPP_TEXT:
			{
				if (message.isLink())
				{
					file << "[ Link: ";
					if (message.getMediaCaption().length() > 0)
					{
						file << message.getMediaCaption() << "; ";
					}
					file << message.getData() << " ]";
				}
				else
				{
					file << message.getData();
				}
			} break;
			case MEDIA_WHATSAPP_IMAGE:
			{
				file << "[ Image: " << message.getFilename();
				if (message.getMediaCaption().length() > 0)
				{
					file << "; " << message.getMediaCaption();
				}
				file << " ]";
			} break;
			case MEDIA_WHATSAPP_AUDIO:
			{
				file << "[ Audio: " << message.getMediaDuration() << "s; " << message.getFilename() << " ]";
			} break;
			case MEDIA_WHATSAPP_VIDEO:
			{
				file << "[ Video: " << message.getFilename();
				if (message.getMediaCaption().length() > 0)
				{
					file << "; " << message.getMediaCaption();
				}
				file << " ]";
			} break;
			case MEDIA_WHATSAPP_CONTACT:
			{
				file << "[ Contact ]";
			} break;
			case MEDIA_WHATSAPP_LOCATION:
			case MEDIA_WHATSAPP_LIVE_LOCATION:
			{
				file << "[ Location: " << message.getLatitude() << "," << message.getLongitude() << " ]";
			} break;
			case MEDIA_WHATSAPP_GIF:
			{
				file << "[ Image (gif/mp4): " << message.getFilename();
				if (message.getMediaCaption().length() > 0)
				{
					file << "; " << message.getMediaCaption();
				}
				file << " ]";
			} break;
		}

		file << std::endl;
	}
}

void ChatExporterTxt::exportChats(const std::vector<WhatsappChat *> &chats, const std::string &filename)
{
	std::ofstream file(filename.c_str());

	if (!file)
	{
		throw Exception("could not open chat export file");
	}

	for (auto i = chats.begin(); i != chats.end(); ++i) {
		WhatsappChat *chat = *i;

		if (i != chats.begin())
			file << std::endl;

		exportChat(*chat, file);
	}
}
