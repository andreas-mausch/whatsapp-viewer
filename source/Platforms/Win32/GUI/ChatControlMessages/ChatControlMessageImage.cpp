#include <windows.h>

#include "ChatControlMessageImage.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, color, dateFont, imageDecoder), height(0)
{
}

ChatControlMessageImage::~ChatControlMessageImage()
{
}

int ChatControlMessageImage::getHeight()
{
	return height;
}

void ChatControlMessageImage::calculateHeight(HWND window)
{
	HDC deviceContext = GetDC(window);

	height = getPreviewBitmapHeight();
	height += getDateHeight(deviceContext);

	ReleaseDC(window, deviceContext);
}

void ChatControlMessageImage::renderInner(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y  + (height - getPreviewBitmapHeight()) / 2);
}
