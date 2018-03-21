#pragma once

#include "ChatControlMessageThumbnail.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageVideo : public ChatControlMessageThumbnail
{
private:
	std::wstring text;

public:
	ChatControlMessageVideo(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageVideo();

};
