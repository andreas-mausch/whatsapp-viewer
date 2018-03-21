#pragma once

#include <vector>
#include "ChatControlMessageThumbnail.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLink : public ChatControlMessageThumbnail
{
private:

public:
	ChatControlMessageLink(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLink();

};
