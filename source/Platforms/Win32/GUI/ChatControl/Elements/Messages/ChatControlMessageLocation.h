#pragma once

#include "ChatControlMessageWithPreviewAndText.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLocation : public ChatControlMessageWithPreviewAndText
{
private:
	double latitude;
	double longitude;

public:
	ChatControlMessageLocation(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLocation();

};
