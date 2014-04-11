#include <windows.h>

#include "ChatControlMessageImage.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageImage::ChatControlMessageImage(WhatsappMessage &message, HGDIOBJ dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessage(message, dateFont), height(0)
{
	bitmap = imageDecoder.loadImage(message.getRawData(), message.getRawDataSize());
}

ChatControlMessageImage::~ChatControlMessageImage()
{
	if (bitmap != NULL)
	{
		DeleteObject(bitmap);
	}
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
	height = 0;

	BITMAP bitmapObject;
	GetObject(bitmap, sizeof(bitmapObject), &bitmapObject);
	height += bitmapObject.bmHeight;

	height += getDateHeight(deviceContext, left, right);

	ReleaseDC(window, deviceContext);
}

void ChatControlMessageImage::renderInner(HDC deviceContext, int y, int left, int right)
{
	BITMAP bitmapObject;
	HDC hdcMem = CreateCompatibleDC(deviceContext);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, bitmap);

	GetObject(bitmap, sizeof(bitmapObject), &bitmapObject);
	BitBlt(deviceContext, left + 5, y + (height - bitmapObject.bmHeight) / 2, bitmapObject.bmWidth, bitmapObject.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}
