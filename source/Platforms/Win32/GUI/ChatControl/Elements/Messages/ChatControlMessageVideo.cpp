#include <windows.h>
#include <sstream>

#include "ChatControlMessageVideo.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

std::string formatVideoText(WhatsappMessage &message)
{
	std::stringstream text;
	text << "Video (" << message.getMediaDuration() << "s)";

	if (message.getMediaCaption().length() > 0)
	{
		text << std::endl << message.getMediaCaption();
	}
	return text.str();
}

ChatControlMessageVideo::ChatControlMessageVideo(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreviewAndText(message, message.getThumbnail(), message.getThumbnailSize(), formatVideoText(message), width, imageDecoder)
{
}

ChatControlMessageVideo::~ChatControlMessageVideo()
{
}
