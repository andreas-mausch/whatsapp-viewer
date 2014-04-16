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

	virtual int calculateHeightInner();
	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight);

public:
	ChatControlMessageLocation(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageLocation();

};
