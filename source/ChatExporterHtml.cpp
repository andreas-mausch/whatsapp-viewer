#include <algorithm>
#include <fstream>
#include <sstream>
#include <windows.h>

#include "ChatExporterHtml.h"
#include "Exceptions/Exception.h"
#include "Platforms/Win32/Timestamp.h"
#include "UTF8/utf8.h"
#include "WhatsApp/Chat.h"
#include "WhatsApp/Emoticons.h"
#include "WhatsApp/Message.h"

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
				output << convertMessageToHtml(message);
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

void ChatExporterHtml::replacePlaceholder(std::string &html, const std::string &placeholder, const std::string &text)
{
	int start = html.find(placeholder);
	html.replace(start, placeholder.length(), text);
}

std::string ChatExporterHtml::convertMessageToHtml(WhatsappMessage &message)
{
	std::string messageString = message.getData();
	std::stringstream output;

	try
	{
		for (std::string::iterator it = messageString.begin(); it != messageString.end();)
		{
			std::string::iterator before = it;
			int character = utf8::next(it, messageString.end());

			if (isSmiley(character))
			{
				output << "<span class=\"emoticon_" << std::hex << character << "\"></span>";
			}
			else
			{
				output << std::string(before, it);
			}
		}
	}
	catch (utf8::exception &exception)
	{
		output << "[INVALID DATA: " << messageString << "]";
	}

	return output.str();
}

void ChatExporterHtml::exportChat(const std::string &filename)
{
	std::string messages = buildMessages();

	std::string contact = chat.getKey();
	if (chat.getSubject().length() > 0)
	{
		contact += "; " + chat.getSubject();
	}

	std::ofstream file(filename.c_str());
	if (!file)
	{
		throw Exception("could not open chat export file");
	}

	std::string html = templateHtml;
	replacePlaceholder(html, "%HEADING%", "WhatsApp Chat");
	replacePlaceholder(html, "%CONTACT%", contact);
	replacePlaceholder(html, "%MESSAGES%", messages);
	replacePlaceholder(html, "%EMOTICON_STYLES%", "");

	file << html;
}
