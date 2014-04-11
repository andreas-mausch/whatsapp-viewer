#pragma once

#include <vector>
#include "ChatControlMessage.h"

class JpegDecoder;
class WhatsappMessage;

class ChatControlMessageImage : public ChatControlMessage
{
private:
	HBITMAP bitmap;
	int height;

public:
	ChatControlMessageImage(WhatsappMessage &message, HGDIOBJ dateFont, JpegDecoder &imageDecoder);
	virtual ~ChatControlMessageImage();

	virtual void calculateHeight(HWND window);
	virtual int getHeight();

	virtual void renderInner(HDC deviceContext, int y, int left, int right);

};
