#pragma once

#include "ChatControlMessageThumbnail.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLocation : public ChatControlMessageThumbnail
{
private:
	double latitude;
	double longitude;

public:
	ChatControlMessageLocation(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLocation();

};
