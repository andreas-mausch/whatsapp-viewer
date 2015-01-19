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

void ChatExporterTxt::exportChat(WhatsappChat &chat, const std::string &filename)
{
	std::ofstream file(filename.c_str());

	if (!file)
	{
		throw Exception("could not open chat export file");
	}

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

		switch (message.getMediaWhatsappType())
		{
			case MEDIA_WHATSAPP_TEXT:
			{
				file << message.getData();
			} break;
			case MEDIA_WHATSAPP_IMAGE:
			{
				file << "[ Image ]";
			} break;
			case MEDIA_WHATSAPP_AUDIO:
			{
				file << "[ Audio ]";
			} break;
			case MEDIA_WHATSAPP_VIDEO:
			{
				file << "[ Video ]";
			} break;
			case MEDIA_WHATSAPP_CONTACT:
			{
				file << "[ Contact ]";
			} break;
			case MEDIA_WHATSAPP_LOCATION:
			{
				file << "[ Location: " << message.getLatitude() << "; " << message.getLongitude() << " ]";
			} break;
		}

		file << std::endl;
	}
}
