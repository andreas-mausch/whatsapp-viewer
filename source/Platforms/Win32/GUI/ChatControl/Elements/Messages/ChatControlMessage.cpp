#include <windows.h>

#include "ChatControlMessage.h"
#include "../../../DrawText.h"
#include "../../../../StringHelper.h"
#include "../../../../Timestamp.h"
#include "../../../../../../WhatsApp/Message.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, int width)
	: message(message), width(width)
{
}

ChatControlMessage::~ChatControlMessage()
{
}

int ChatControlMessage::getWidth()
{
	return width;
}

void ChatControlMessage::updateWidth(HWND window, int width)
{
	this->width = width;
}

WhatsappMessage &ChatControlMessage::getMessage()
{
	return message;
}
