#pragma once

#include "ChatControlMessage.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLocation : public ChatControlMessage
{
private:
	HBITMAP bitmap;
	int height;
	double latitude;
	double longitude;

public:
	ChatControlMessageLocation(WhatsappMessage &message, HGDIOBJ dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLocation();

	virtual void calculateHeight(HWND window);
	virtual int getHeight();

	virtual void renderInner(HDC deviceContext, int y, int left, int right, int clientHeight);

};
