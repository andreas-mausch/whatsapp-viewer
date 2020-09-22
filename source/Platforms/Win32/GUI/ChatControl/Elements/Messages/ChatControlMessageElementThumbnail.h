#pragma once

#include "ChatControlMessageElement.h"

class Bitmap;
class ImageDecoder;
class Smileys;
class WhatsappMessage;

class ChatControlMessageElementThumbnail : public ChatControlMessageElement
{
private:
	Bitmap *thumbnail;
	int thumbnailWidth;
	int thumbnailHeight;

	void renderThumbnail(HDC deviceContext, int x, int y);

public:
	ChatControlMessageElementThumbnail(unsigned char *data, int size, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageElementThumbnail();

	virtual void calculateHeight(int width);
	virtual void render(HDC deviceContext, int x, int y, int clientHeight, Smileys &smileys);

};
