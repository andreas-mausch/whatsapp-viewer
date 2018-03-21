#pragma once

#include "ChatControlMessage.h"

class Bitmap;
class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageThumbnail : public ChatControlMessage
{
private:
	std::string text;
	WCHAR *wcharText;

	Bitmap *thumbnail;
	int thumbnailWidth;
	int thumbnailHeight;

	int getThumbnailWidth();
	int getThumbnailHeight();
	void renderThumbnail(HDC deviceContext, int x, int y);

public:
	ChatControlMessageThumbnail(WhatsappMessage &message, unsigned char *data, int size, const std::string &text, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageThumbnail();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
