#include <windows.h>

#include "ChatControlMessageLocation.h"
#include "../DrawText.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageLocation::ChatControlMessageLocation(WhatsappMessage &message, HFONT dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, dateFont, imageDecoder), height(0)
{
	latitude = message.getLatitude();
	longitude = message.getLongitude();
	swprintf(text, 256, L"%f, %f", latitude, longitude);
}

ChatControlMessageLocation::~ChatControlMessageLocation()
{
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

	height += getPreviewBitmapHeight();

	height += calculateDrawTextHeight(deviceContext, text, right - left, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	height += 10;

	height += getDateHeight(deviceContext, left, right);

	ReleaseDC(window, deviceContext);
}

void ChatControlMessageLocation::renderInner(HDC deviceContext, int y, int left, int right, int clientHeight)
{
	renderPreviewBitmap(deviceContext, left + 5, y + 5);

	SetTextColor(deviceContext, RGB(0, 0, 0));
	drawText(deviceContext, text, left, y + getPreviewBitmapHeight() + 10, right - left, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
}
