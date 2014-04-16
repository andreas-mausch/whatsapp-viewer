#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageVideo : public ChatControlMessageWithPreview
{
private:
	std::wstring text;

	virtual int calculateHeightInner();
	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight);

public:
	ChatControlMessageVideo(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageVideo();

};
