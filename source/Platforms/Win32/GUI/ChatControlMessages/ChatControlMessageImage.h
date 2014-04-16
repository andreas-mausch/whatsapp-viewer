#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageImage : public ChatControlMessageWithPreview
{
private:
	int height;

public:
	ChatControlMessageImage(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageImage();

	virtual void calculateHeight(HWND window);
	virtual int getHeight();

	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight);

};
