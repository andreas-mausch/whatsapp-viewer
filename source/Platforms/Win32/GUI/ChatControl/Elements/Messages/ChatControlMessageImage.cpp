#include <windows.h>
#include <sstream>

#include "ChatControlMessageImage.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

std::string buildImageCaption(WhatsappMessage &message)
{
	return message.getMediaCaption();
}

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageThumbnail(message, message.getThumbnail(), message.getThumbnailSize(), buildImageCaption(message), width, imageDecoder)
{
}

ChatControlMessageImage::~ChatControlMessageImage()
{
}
