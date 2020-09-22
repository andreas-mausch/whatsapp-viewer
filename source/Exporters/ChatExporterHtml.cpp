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
#include "../Platforms/Win32/GUI/ChatControl/Elements/Messages/ChatControlMessage.h"
#include "../WhatsApp/Chat.h"
#include "../WhatsApp/Emoticons.h"
#include "../WhatsApp/Message.h"
#include "../Settings.h"
#include "../../resources/resource.h"

ChatExporterHtml::ChatExporterHtml(Settings &settings, const std::string &templateHtml)
	: settings(settings), templateHtml(templateHtml)
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
					output << "<div><span>" << convertMessageToHtml(message.getMediaCaption(), usedEmoticons) << "</span></div>";
					output << "<div><span>" << convertMessageToHtml(message.getData(), usedEmoticons) << "</span></div>";
				}
				else
				{
					output << "<span>" << convertMessageToHtml(message.getData(), usedEmoticons) << "</span>";
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
					output << "<div><span>" << convertMessageToHtml(message.getMediaCaption(), usedEmoticons) << "</span></div>";
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
					output << "<div><span>" << convertMessageToHtml(message.getMediaCaption(), usedEmoticons) << "</span></div>";
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

std::string ChatExporterHtml::convertMessageToHtml(std::string message, std::set<int> &usedEmoticons)
{
	std::stringstream output;

	try
	{
		for (std::string::iterator it = message.begin(); it != message.end();)
		{
			std::string::iterator before = it;
			int character = utf8::next(it, message.end());
			int emoticonCharacter = getSmiley(character);

			if (emoticonCharacter > 0)
			{
				usedEmoticons.insert(emoticonCharacter);
				output.imbue(std::locale::classic());
				output << "<span class=\"emoticon_" << std::hex << emoticonCharacter << "\"></span>";
				output.imbue(std::locale());
			}
			else
			{
				output << std::string(before, it);
			}
		}
	}
	catch (utf8::exception &exception)
	{
		output << "[INVALID DATA: " << message << "]";
	}

	return output.str();
}

std::string ChatExporterHtml::buildEmoticonStyles(const std::set<int> &usedEmoticons)
{
	std::stringstream css;

	for(std::set<int>::const_iterator it = usedEmoticons.begin(); it != usedEmoticons.end(); ++it)
	{
		int character = *it;

		unsigned char *bytes = NULL;
		DWORD size = 0;
		loadResource(MAKEINTRESOURCE(characterToResource[character]), L"PNG", bytes, size);
		std::string base64Emoticon = base64_encode(bytes, size);

		css.imbue(std::locale::classic());
		css << ".emoticon_" << std::hex << character << " {" << std::endl;
		css.imbue(std::locale());
		css << "display: inline-block;" << std::endl;
		css << "width: 20px;" << std::endl;
		css << "height: 20px;" << std::endl;
		css << "background-image: url(data:image/png;base64," << base64Emoticon << ")" << std::endl;
		css << "}" << std::endl;
	}

	return css.str();
}

void ChatExporterHtml::exportChat(WhatsappChat &chat, const std::string &filename)
{
	std::set<int> usedEmoticons;
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

	std::ofstream file(filename.c_str());
	if (!file)
	{
		throw Exception("could not open chat export file");
	}

	std::string html = templateHtml;
	std::string heading = "WhatsApp Chat";
	replacePlaceholder(html, "%HEADING%", heading);
	replacePlaceholder(html, "%TITLE%", heading + " " + contact);
	replacePlaceholder(html, "%CONTACT%", contact);
	replacePlaceholder(html, "%CONTACT_NAME%", contactName);
	replacePlaceholder(html, "%MESSAGES%", messages);
	replacePlaceholder(html, "%EMOTICON_STYLES%", buildEmoticonStyles(usedEmoticons));

	file << html;
}
