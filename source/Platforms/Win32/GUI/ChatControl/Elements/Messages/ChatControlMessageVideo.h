#pragma once

#include "ChatControlMessageWithPreviewAndText.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageVideo : public ChatControlMessageWithPreviewAndText
{
private:
	std::wstring text;

public:
	ChatControlMessageVideo(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageVideo();

};
