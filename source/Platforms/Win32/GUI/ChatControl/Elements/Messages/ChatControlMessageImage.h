#pragma once

#include "ChatControlMessageThumbnail.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageImage : public ChatControlMessageThumbnail
{
public:
	ChatControlMessageImage(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageImage();

};
