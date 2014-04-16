#include <windows.h>

#include "ChatControlMessageImage.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, color, dateFont, imageDecoder)
{
}

ChatControlMessageImage::~ChatControlMessageImage()
{
}

int ChatControlMessageImage::calculateHeightInner()
{
	return getPreviewBitmapHeight();
}

void ChatControlMessageImage::renderInner(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y  + (getHeight() - getPreviewBitmapHeight()) / 2);
}
