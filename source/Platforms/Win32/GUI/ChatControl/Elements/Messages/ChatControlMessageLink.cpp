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

unsigned char *getThumbnail(WhatsappMessage &message)
{
	if (message.getThumbImage() == NULL)
	{
		return NULL;
	}

	unsigned char *image = message.getThumbImage() + 27;

	if (image[0] == 0xFF && image[1] == 0xD8)
	{
		return image;
	}

	return NULL;
}

int getThumbnailSize(WhatsappMessage &message)
{
	unsigned char *thumbnail = getThumbnail(message);

	if (thumbnail == NULL)
	{
		return 0;
	}

	return (thumbnail - message.getThumbImage()) + message.getThumbImageSize();
}

ChatControlMessageLink::ChatControlMessageLink(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreviewAndText(message, getThumbnail(message), getThumbnailSize(message), buildText(message), width, imageDecoder)
{
}

ChatControlMessageLink::~ChatControlMessageLink()
{
}
