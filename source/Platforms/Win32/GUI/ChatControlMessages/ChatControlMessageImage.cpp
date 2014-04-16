#include <windows.h>

#include "ChatControlMessageImage.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, HFONT dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, dateFont, imageDecoder), height(0)
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
	RECT clientRect;
	GetClientRect(window, &clientRect);

	HDC deviceContext = GetDC(window);

	int gap = 40;
	int left = 10;
	int right = clientRect.right - gap - 10;

	height = getPreviewBitmapHeight();
	height += getDateHeight(deviceContext, left, right);

	ReleaseDC(window, deviceContext);
}

void ChatControlMessageImage::renderInner(HDC deviceContext, int y, int left, int right, int clientHeight)
{
	renderPreviewBitmap(deviceContext, left + 5, y + (height - getPreviewBitmapHeight()) / 2);
}
