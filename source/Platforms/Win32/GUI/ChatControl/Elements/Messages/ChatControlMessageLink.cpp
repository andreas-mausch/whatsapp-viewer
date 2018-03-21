#include <sstream>
#include <windows.h>
#include <sstream>

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
	: ChatControlMessageThumbnail(message, message.getLinkThumbnail(), message.getLinkThumbnailSize(), buildText(message), width, imageDecoder)
{
}

ChatControlMessageLink::~ChatControlMessageLink()
{
}
