#include <algorithm>
#include <fstream>
#include <sstream>
#include <windows.h>

#include "ChatExporterHtml.h"
#include "../Exceptions/Exception.h"
#include "../Libraries/Base64/Base64.h"
#include "../Libraries/UTF8/utf8.h"
#include "../Platforms/Win32/ImageDecoder.h"
#include "../Platforms/Win32/Resource.h"
#include "../Platforms/Win32/Timestamp.h"
#include "../Platforms/Win32/GUI/SmileyList.h"
#include "../Platforms/Win32/GUI/ChatControl/Elements/Messages/ChatControlMessageAudio.h"
#include "../WhatsApp/Chat.h"
#include "../WhatsApp/Emoticons.h"
#include "../WhatsApp/Message.h"
#include "../Settings.h"
#include "../../resources/resource.h"

ChatExporterHtml::ChatExporterHtml(Settings &settings, const std::string &templateHtml,
		const std::string &entryTemplateHtml)
	: settings(settings), templateHtml(templateHtml), entryTemplateHtml(entryTemplateHtml)
{
}

ChatExporterHtml::~ChatExporterHtml()
{
}

std::string ChatExporterHtml::buildMessages(WhatsappChat &chat, std::set<int> &usedEmoticons)
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

		output << "\">";

		if (message.getQuotedMessage() != NULL)
		{
			output << "<div class=\"quote\">";
			output << "<div><span>Quote</span></div>";
			output << "<div><span>At: " << formatTimestamp(message.getQuotedMessage()->getTimestamp()) << "</span></div>";
			output << "<div><span>Message ID: " << message.getQuotedMessage()->getMessageId() << "</span></div>";
			output << "</div>";
		}

		output << "<div class=\"text\">";

		switch (message.getMediaWhatsappType())
		{
			case MEDIA_WHATSAPP_TEXT:
			{
				if (message.isLink())
				{
					if (message.hasLinkThumbnail())
					{
						output << "<div><img src=\"data:image/jpeg;base64," << base64_encode(message.getLinkThumbnail(), message.getLinkThumbnailSize()) << "\"></div>" << std::endl;
					}
					output << "<div><span>" << message.getMediaCaption() << "</span></div>";
					output << "<div><span>" << message.getData() << "</span></div>";
				}
				else
				{
					output << "<span>" << convertMessageToHtml(message, usedEmoticons) << "</span>";
				}
			} break;
			case MEDIA_WHATSAPP_IMAGE:
			case MEDIA_WHATSAPP_GIF:
			{
				if (message.hasThumbnail())
				{
					output << "<div><img src=\"data:image/jpeg;base64," << base64_encode(message.getThumbnail(), message.getThumbnailSize()) << "\"></div>" << std::endl;
				}
				if (message.getMediaCaption().length() > 0)
				{
					output << "<div><span>" << message.getMediaCaption() << "</span></div>";
				}
			} break;
			case MEDIA_WHATSAPP_AUDIO:
			{
				output << "<span>[ " << formatAudio(message) << " ]</span>";
			} break;
			case MEDIA_WHATSAPP_VIDEO:
			{
				if (message.hasThumbnail())
				{
					output << "<div><img src=\"data:image/jpeg;base64," << base64_encode(message.getThumbnail(), message.getThumbnailSize()) << "\"></div>" << std::endl;
				}
				if (message.getMediaCaption().length() > 0)
				{
					output << "<div><span>" << message.getMediaCaption() << "</span></div>";
				}
				output << "<span>[ Video ]</span>";
			} break;
			case MEDIA_WHATSAPP_CONTACT:
			{
				output << "<span>[ Contact ]</span>";
			} break;
			case MEDIA_WHATSAPP_LOCATION:
			case MEDIA_WHATSAPP_LIVE_LOCATION:
			{
				if (message.hasThumbnail())
				{
					output << "<div><img src=\"data:image/jpeg;base64," << base64_encode(message.getThumbnail(), message.getThumbnailSize()) << "\"></div>" << std::endl;
				}
				output << "<span>[ Location: " << message.getLatitude() << "," << message.getLongitude() << " ]</span>";
			} break;
		}

		output << "</div>";
		output << "<div class=\"footer\">";

		if (message.getFilename().length() > 0)
		{
			output << "<div class=\"filename\"><span>" << message.getFilename() << "</span></div>";
		}

		if (message.getRemoteResource().size() > 0)
		{
			output << "<div class=\"remote-resource\"><span>" << settings.findDisplayName(message.getRemoteResource()) << "</span> (<span>" << message.getRemoteResource() << "</span>)</div>";
		}

		output << "<div class=\"timestamp\"><span>" << formatTimestamp(message.getTimestamp()) << "</span></div>";
		output << "</div></div>" << std::endl;
	}

	return output.str();
}

void ChatExporterHtml::replacePlaceholder(std::string &html, const std::string &placeholder, const std::string &text)
{
	int start = html.find(placeholder);
	html.replace(start, placeholder.length(), text);
}

std::string ChatExporterHtml::convertMessageToHtml(WhatsappMessage &message, std::set<int> &usedEmoticons)
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
				usedEmoticons.insert(character);
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

std::string ChatExporterHtml::buildEmoticonStyles(const std::set<int> &usedEmoticons)
{
	std::stringstream css;
	for(std::set<int>::const_iterator it = usedEmoticons.begin(); it != usedEmoticons.end(); ++it)
	{
		int character = *it;
		int smileyIndex = -1;

		for (int i = 0; i < smileyCount; i++)
		{
			if (smileyList[i].character == character)
			{
				smileyIndex = i;
				break;
			}
		}

		if (smileyIndex >= 0)
		{
			unsigned char *bytes = NULL;
			DWORD size = 0;
			loadResource(MAKEINTRESOURCE(smileyList[smileyIndex].resource), L"PNG", bytes, size);
			std::string base64Emoticon = base64_encode(bytes, size);

			css << ".emoticon_" << std::hex << character << " {" << std::endl;
			css << "display: inline-block;" << std::endl;
			css << "width: 20px;" << std::endl;
			css << "height: 20px;" << std::endl;
			css << "background-image: url(data:image/png;base64," << base64Emoticon << ")" << std::endl;
			css << "}" << std::endl;
		}
	}

	return css.str();
}

std::string ChatExporterHtml::exportChat(WhatsappChat &chat, std::set<int> &usedEmoticons)
{
	std::string messages = buildMessages(chat, usedEmoticons);

	std::string contact = chat.getKey();
	if (chat.getSubject().length() > 0)
	{
		contact += "; " + chat.getSubject();
	}

	std::string contactName = chat.getDisplayName();

	if (contactName == contact)
	{
		contactName = "";
	}

	std::string html = entryTemplateHtml;
	std::string heading = "WhatsApp Chat";
	replacePlaceholder(html, "%HEADING%", heading);
	replacePlaceholder(html, "%CONTACT%", contact);
	replacePlaceholder(html, "%CONTACT_NAME%", contactName);
	replacePlaceholder(html, "%MESSAGES%", messages);
	return html;
}

void ChatExporterHtml::exportChats(const std::vector<WhatsappChat *> &chats, const std::string &filename)
{
	std::ofstream file(filename.c_str());
	if (!file)
	{
		throw Exception("could not open chat export file");
	}

	std::set<int> usedEmoticons;

	std::string chatEntries;
	for (auto i = chats.begin(); i != chats.end(); ++i) {
		WhatsappChat *chat = *i;

		if (i != chats.begin())
			chatEntries += "\n";

		chatEntries += exportChat(*chat, /* inout */ usedEmoticons);
	}

	std::string html = templateHtml;
	std::string heading = "WhatsApp Chat";
	std::string title = (chats.size() == 1) ? (heading + " " + chats[0]->getKey()) : heading;
	replacePlaceholder(html, "%TITLE%", title);
	replacePlaceholder(html, "%EMOTICON_STYLES%", buildEmoticonStyles(usedEmoticons));
	replacePlaceholder(html, "%CHAT_ENTRIES%", chatEntries);

	file << html;
}
