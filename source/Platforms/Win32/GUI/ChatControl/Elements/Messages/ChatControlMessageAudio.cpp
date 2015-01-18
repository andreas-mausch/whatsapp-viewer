#include <windows.h>

#include "ChatControlMessageAudio.h"
#include "ChatControlMessageTextElement.h"
#include "../../../DrawText.h"
#include "../../../../../../WhatsApp/Message.h"

ChatControlMessageAudio::ChatControlMessageAudio(WhatsappMessage &message, int width, Smileys &smileys)
	: ChatControlMessage(message, width), smileys(smileys)
{
	textElement = new ChatControlMessageTextElement("Audio");
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
