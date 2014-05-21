#include <windows.h>

#include "ChatControlMessageImage.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, imageDecoder)
{
}

ChatControlMessageImage::~ChatControlMessageImage()
{
}

int ChatControlMessageImage::calculateHeight()
{
	return getPreviewBitmapHeight() + 10;
}

void ChatControlMessageImage::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y + 5);
}
