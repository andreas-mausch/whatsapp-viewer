#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageImage : public ChatControlMessageWithPreview
{
private:
	int height;

public:
	ChatControlMessageImage(WhatsappMessage &message, HFONT dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageImage();

	virtual void calculateHeight(HWND window);
	virtual int getHeight();

	virtual void renderInner(HDC deviceContext, int y, int left, int right, int clientHeight);

};
