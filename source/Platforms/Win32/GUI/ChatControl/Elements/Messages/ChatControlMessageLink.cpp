#include <sstream>
#include <windows.h>

#include "ChatControlMessageLink.h"
#include "../../../../ImageDecoder.h"
#include "../../../../../../WhatsApp/Message.h"

std::string buildText(WhatsappMessage &message)
{
	std::stringstream text;
	text << message.getMediaCaption() << std::endl << message.getData();
	return text.str();
}

ChatControlMessageLink::ChatControlMessageLink(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreviewAndText(message, buildText(message), width, imageDecoder)
{
}

ChatControlMessageLink::~ChatControlMessageLink()
{
}
