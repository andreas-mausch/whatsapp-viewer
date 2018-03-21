#include <windows.h>
#include <sstream>

#include "ChatControlMessageLocation.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

std::string buildLocationString(WhatsappMessage &message)
{
	std::stringstream location;
	location << message.getLatitude() << ", " << message.getLongitude();
	return location.str();
}

ChatControlMessageLocation::ChatControlMessageLocation(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageThumbnail(message, message.getThumbnail(), message.getThumbnailSize(), buildLocationString(message), width, imageDecoder)
{
	latitude = message.getLatitude();
	longitude = message.getLongitude();
}

ChatControlMessageLocation::~ChatControlMessageLocation()
{
}
