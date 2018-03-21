#pragma once

#include <vector>
#include "ChatControlMessageWithPreviewAndText.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLink : public ChatControlMessageWithPreviewAndText
{
private:

public:
	ChatControlMessageLink(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLink();

};
