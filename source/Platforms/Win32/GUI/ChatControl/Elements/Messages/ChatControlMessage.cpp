#include <sstream>
#include <windows.h>

#include "ChatControlMessage.h"
#include "ChatControlMessageElement.h"
#include "ChatControlMessageElementSmiley.h"
#include "ChatControlMessageElementText.h"
#include "ChatControlMessageElementThumbnail.h"
#include "../../../SmileyList.h"
#include "../../../../../../Libraries/UTF8/utf8.h"
#include "../../../../../../WhatsApp/Emoticons.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../../../VectorUtils.h"
#include "../../../../StringHelper.h"
#include "../../../../Timestamp.h"

std::string formatAudio(WhatsappMessage& message)
{
	std::stringstream text;
	text << "Audio (" << message.getMediaDuration() << "s)";

	if (message.getMediaName().length() > 0)
	{
		text << " : " << message.getMediaName();
	}

	return text.str();
}

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, int width, ImageDecoder &imageDecoder, Smileys &smileys)
	: message(message), width(width), smileys(smileys)
{
	if (message.getQuotedMessage() != NULL)
	{
		std::stringstream quoted;
		quoted << "Quoted message from " << formatTimestamp(message.getQuotedMessage()->getTimestamp()) << ": " << message.getQuotedMessage()->getMessageId();
		addText(quoted.str());
	}

	if (message.hasLinkThumbnail())
	{
		addThumbnail(message.getLinkThumbnail(), message.getLinkThumbnailSize(), imageDecoder);
	}
	if (message.hasThumbnail())
	{
		addThumbnail(message.getThumbnail(), message.getThumbnailSize(), imageDecoder);
	}
	addText(message.getMediaCaption());
	addText(message.getData());

	switch (message.getMediaWhatsappType())
	{
		case MEDIA_WHATSAPP_AUDIO:
		{
			addText(formatAudio(message));
		} break;
		case MEDIA_WHATSAPP_VIDEO:
		{
			std::stringstream text;
			text << "Video (" << message.getMediaDuration() << "s)";
			addText(text.str());
		} break;
		case MEDIA_WHATSAPP_LOCATION:
		case MEDIA_WHATSAPP_LIVE_LOCATION:
		{
			std::stringstream text;
			text << message.getLatitude() << ", " << message.getLongitude();
			addText(text.str());
		} break;
	}
}

ChatControlMessage::~ChatControlMessage()
{
	clearVector(elements);
}

int ChatControlMessage::calculateHeight()
{
	int height = 0;

	for (auto &element: elements)
	{
		element->calculateHeight(getWidth());
		height += element->getHeight();
	}

	return height;
}

void ChatControlMessage::addText(std::string message)
{
	if (message.empty())
	{
		return;
	}
	try
	{
		int lastSplit = 0;
		for (std::string::iterator it = message.begin(); it != message.end();)
		{
			bool begin = (it - message.begin()) == lastSplit;
			std::string::iterator before = it;
			int character = utf8::next(it, message.end());
			int emoticonCharacter = getSmiley(character);

			if (emoticonCharacter > 0)
			{
				if (!begin)
				{
					int start = lastSplit;
					int end = before - message.begin();
					std::string leftPart = message.substr(start, end - start);
					elements.push_back(new ChatControlMessageElementText(leftPart));
				}

				elements.push_back(new ChatControlMessageElementSmiley(emoticonCharacter));

				lastSplit = (it - message.begin());
			}
		}

		int length = message.length();
		if (lastSplit < length)
		{
			std::string part = message.substr(lastSplit, length);
			elements.push_back(new ChatControlMessageElementText(part));
		}
	}
	catch (utf8::exception &exception)
	{
		std::stringstream invalidDataString;
		invalidDataString << "[INVALID DATA: " << message << "]";
		elements.push_back(new ChatControlMessageElementText(invalidDataString.str()));
	}
}

void ChatControlMessage::addThumbnail(unsigned char *data, int size, ImageDecoder &imageDecoder)
{
	elements.push_back(new ChatControlMessageElementThumbnail(data, size, imageDecoder));
}

void ChatControlMessage::render(HDC deviceContext, int x, int y, int clientHeight)
{
	SetTextColor(deviceContext, RGB(0, 0, 0));

	for (auto &element: elements)
	{
		if (y + element->getHeight() > 0)
		{
			element->render(deviceContext, x, y, getWidth(), smileys);
		}
		y += element->getHeight();

		if (y > clientHeight)
		{
			break;
		}
	}
}

int ChatControlMessage::getWidth()
{
	return width;
}

void ChatControlMessage::updateWidth(HWND window, int width)
{
	this->width = width;
}

WhatsappMessage& ChatControlMessage::getMessage()
{
	return message;
}
