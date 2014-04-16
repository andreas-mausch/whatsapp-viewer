#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageImage : public ChatControlMessageWithPreview
{
private:
	virtual int calculateHeightInner();
	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight);

public:
	ChatControlMessageImage(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageImage();

};
