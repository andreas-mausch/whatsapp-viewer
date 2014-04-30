#pragma once

#include "ChatControlMessage.h"

class Bitmap;
class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageWithPreview : public ChatControlMessage
{
private:
	Bitmap *bitmap;
	int bitmapWidth;
	int bitmapHeight;

public:
	ChatControlMessageWithPreview(WhatsappMessage &message, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageWithPreview();

	void renderPreviewBitmap(HDC deviceContext, int x, int y);
	int getPreviewBitmapWidth();
	int getPreviewBitmapHeight();

};
