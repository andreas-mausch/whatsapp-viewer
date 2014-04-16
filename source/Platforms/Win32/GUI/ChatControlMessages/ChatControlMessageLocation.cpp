#include <windows.h>

#include "ChatControlMessageLocation.h"
#include "../DrawText.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageLocation::ChatControlMessageLocation(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, color, dateFont, imageDecoder), height(0)
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
	HDC deviceContext = GetDC(window);

	height = getPreviewBitmapHeight();

	height += calculateDrawTextHeight(deviceContext, text, width, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	height += 10;

	height += getDateHeight(deviceContext);

	ReleaseDC(window, deviceContext);
}

void ChatControlMessageLocation::renderInner(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y + 5);

	SetTextColor(deviceContext, RGB(0, 0, 0));
	drawText(deviceContext, text, x, y + getPreviewBitmapHeight() + 10, width, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
}
