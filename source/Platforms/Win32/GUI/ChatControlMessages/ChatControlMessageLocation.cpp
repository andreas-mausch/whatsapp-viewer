#include <windows.h>

#include "ChatControlMessageLocation.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageLocation::ChatControlMessageLocation(WhatsappMessage &message, HGDIOBJ dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessage(message, dateFont), height(0)
{
	bitmap = imageDecoder.loadImage(message.getRawData(), message.getRawDataSize());
	latitude = message.getLatitude();
	longitude = message.getLongitude();
}

ChatControlMessageLocation::~ChatControlMessageLocation()
{
	if (bitmap != NULL)
	{
		DeleteObject(bitmap);
	}
}

int ChatControlMessageLocation::getHeight()
{
	return height;
}

void ChatControlMessageLocation::calculateHeight(HWND window)
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

	RECT textRect = { left, 0, right, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
	WCHAR text[256];
	swprintf(text, 256, L"%f, %f", latitude, longitude);
	DrawText(deviceContext, text, -1, &textRect, DT_CALCRECT | DT_WORDBREAK);
	SelectObject(deviceContext, oldFont);
	height += textRect.bottom - textRect.top + 10;

	height += getDateHeight(deviceContext, left, right);

	ReleaseDC(window, deviceContext);
}

void ChatControlMessageLocation::renderInner(HDC deviceContext, int y, int left, int right, int clientHeight)
{
	BITMAP bitmapObject;
	HDC hdcMem = CreateCompatibleDC(deviceContext);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, bitmap);

	GetObject(bitmap, sizeof(bitmapObject), &bitmapObject);
	BitBlt(deviceContext, left + 5, y + 5, bitmapObject.bmWidth, bitmapObject.bmHeight, hdcMem, 0, 0, SRCCOPY);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);

	SetTextColor(deviceContext, RGB(0, 0, 0));
	RECT textRect = { left, y + bitmapObject.bmHeight + 10, right, y + bitmapObject.bmHeight + 10 };
	HGDIOBJ oldFont = SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
	WCHAR text[256];
	swprintf(text, 256, L"%f, %f", latitude, longitude);
	DrawText(deviceContext, text, -1, &textRect, DT_CALCRECT | DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
	textRect.right = right;
	DrawText(deviceContext, text, -1, &textRect, DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
	SelectObject(deviceContext, oldFont);
}
