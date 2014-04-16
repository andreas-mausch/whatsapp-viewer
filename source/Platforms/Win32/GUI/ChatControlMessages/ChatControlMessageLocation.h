#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageLocation : public ChatControlMessageWithPreview
{
private:
	WCHAR text[256];
	int height;
	double latitude;
	double longitude;

public:
	ChatControlMessageLocation(WhatsappMessage &message, HFONT dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLocation();

	virtual void calculateHeight(HWND window);
	virtual int getHeight();

	virtual void renderInner(HDC deviceContext, int y, int left, int right, int clientHeight);

};
