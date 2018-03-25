#include <windows.h>
#include <sstream>

#include "ChatControlMessageImage.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

std::string buildImageCaption(WhatsappMessage &message)
{
	std::stringstream text;
	text << message.getMediaCaption();

	if (message.getFilename().length() > 0)
	{
		if (message.getMediaCaption().length() > 0)
		{
			text << std::endl;
		}
		text << message.getFilename();
	}

	return text.str();
}

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageThumbnail(message, message.getThumbnail(), message.getThumbnailSize(), buildImageCaption(message), width, imageDecoder)
{
}

ChatControlMessageImage::~ChatControlMessageImage()
{
}
