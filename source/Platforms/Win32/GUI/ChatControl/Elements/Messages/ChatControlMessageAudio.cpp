#include <sstream>
#include <windows.h>

#include "ChatControlMessageAudio.h"
#include "ChatControlMessageTextElement.h"
#include "../../../DrawText.h"
#include "../../../../../../WhatsApp/Message.h"

std::string formatAudio(WhatsappMessage &message)
{
	std::stringstream text;
	text << "Audio (" << message.getMediaDuration() << "s)";

	if (message.getMediaName().length() > 0)
	{
		text << " : " << message.getMediaName();
	}

	return text.str();
}

ChatControlMessageAudio::ChatControlMessageAudio(WhatsappMessage &message, int width, Smileys &smileys)
	: ChatControlMessage(message, width), smileys(smileys)
{
	textElement = new ChatControlMessageTextElement(formatAudio(message));
}

ChatControlMessageAudio::~ChatControlMessageAudio()
{
	delete textElement;
}

int ChatControlMessageAudio::calculateHeight()
{
	textElement->calculateHeight(getWidth());
	return textElement->getHeight();
}

void ChatControlMessageAudio::render(HDC deviceContext, int x, int y, int clientHeight)
{
	SetTextColor(deviceContext, RGB(0, 0, 0));
	textElement->render(deviceContext, x, y, getWidth(), smileys);
}
