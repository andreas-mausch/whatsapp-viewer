#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLocation : public ChatControlMessageWithPreview
{
private:
	WCHAR text[256];
	double latitude;
	double longitude;

public:
	ChatControlMessageLocation(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLocation();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
