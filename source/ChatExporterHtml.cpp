#include <algorithm>
#include <fstream>
#include <sstream>
#include <windows.h>

#include "ChatExporterHtml.h"
#include "Exceptions/Exception.h"
#include "WhatsApp/Chat.h"
#include "WhatsApp/Message.h"
#include "Platforms/Win32/Timestamp.h"

ChatExporterHtml::ChatExporterHtml(const std::string &templateHtml, WhatsappChat &chat)
	: chat(chat), templateHtml(templateHtml)
{
}

ChatExporterHtml::~ChatExporterHtml()
{
}

std::string ChatExporterHtml::buildMessages()
{
	std::stringstream output;

	std::vector<WhatsappMessage *> &messages = chat.getMessages(true);
	for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		WhatsappMessage &message = **it;

		output << "<div class=\"message ";

		if (message.isFromMe())
		{
			output << "outgoing_message";
		}
		else
		{
			output << "incoming_message";
		}

		output << "\"><span>";

		switch (message.getMediaWhatsappType())
		{
			case MEDIA_WHATSAPP_TEXT:
			{
				output << message.getData();
			} break;
			case MEDIA_WHATSAPP_IMAGE:
			{
				output << "[ Image ]";
			} break;
			case MEDIA_WHATSAPP_AUDIO:
			{
				output << "[ Audio ]";
			} break;
			case MEDIA_WHATSAPP_VIDEO:
			{
				output << "[ Video ]";
			} break;
			case MEDIA_WHATSAPP_CONTACT:
			{
				output << "[ Contact ]";
			} break;
			case MEDIA_WHATSAPP_LOCATION:
			{
				output << "[ Location: " << message.getLatitude() << "; " << message.getLongitude() << " ]";
			} break;
		}

		output << "</span></div>";

		output << std::endl;
	}

	return output.str();
}

void ChatExporterHtml::exportChat(const std::string &filename)
{
	std::string messages = buildMessages();
	std::ofstream file(filename.c_str());

	if (!file)
	{
		throw Exception("could not open chat export file");
	}

	std::string html = templateHtml;
	std::string replaceParameter = "%MESSAGES%";

	int start = html.find(replaceParameter);
	html.replace(start, replaceParameter.length(), messages);

	file << html;
}
