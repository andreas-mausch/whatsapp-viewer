#pragma once

#include "ChatControlMessageWithPreviewAndText.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageImage : public ChatControlMessageWithPreviewAndText
{
public:
	ChatControlMessageImage(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageImage();

};
