#include <sstream>
#include <windows.h>

#include "ChatControlMessage2.h"
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

ChatControlMessage2::ChatControlMessage2(WhatsappMessage &message, int width, ImageDecoder &imageDecoder, Smileys &smileys)
	: ChatControlMessage(message, width), smileys(smileys)
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

ChatControlMessage2::~ChatControlMessage2()
{
	clearVector(elements);
}

int ChatControlMessage2::calculateHeight()
{
	int height = 0;

	for (auto &element: elements)
	{
		element->calculateHeight(getWidth());
		height += element->getHeight();
	}

	return height;
}

void ChatControlMessage2::addText(std::string message)
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

void ChatControlMessage2::addThumbnail(unsigned char *data, int size, ImageDecoder &imageDecoder)
{
	elements.push_back(new ChatControlMessageElementThumbnail(data, size, imageDecoder));
}

void ChatControlMessage2::render(HDC deviceContext, int x, int y, int clientHeight)
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
