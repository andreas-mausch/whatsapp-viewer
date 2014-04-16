#include <windows.h>

#include "ChatControlMessageLocation.h"
#include "../DrawText.h"
#include "../../../../WhatsApp/Message.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageLocation::ChatControlMessageLocation(WhatsappMessage &message, int width, int color, HFONT dateFont, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, color, dateFont, imageDecoder)
{
	latitude = message.getLatitude();
	longitude = message.getLongitude();
	swprintf(text, 256, L"%f, %f", latitude, longitude);
}

ChatControlMessageLocation::~ChatControlMessageLocation()
{
}

int ChatControlMessageLocation::calculateHeightInner()
{
	int height = getPreviewBitmapHeight();

	HDC deviceContext = GetDC(NULL);
	height += calculateDrawTextHeight(deviceContext, text, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	ReleaseDC(NULL, deviceContext);

	height += 10;

	return height;
}

void ChatControlMessageLocation::renderInner(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y + 5);

	SetTextColor(deviceContext, RGB(0, 0, 0));
	drawText(deviceContext, text, x, y + getPreviewBitmapHeight() + 10, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
}
