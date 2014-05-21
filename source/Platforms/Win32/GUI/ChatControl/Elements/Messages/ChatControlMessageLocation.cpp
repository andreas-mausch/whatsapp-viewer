#include <windows.h>

#include "ChatControlMessageLocation.h"
#include "../../../DrawText.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

ChatControlMessageLocation::ChatControlMessageLocation(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, imageDecoder)
{
	latitude = message.getLatitude();
	longitude = message.getLongitude();
	swprintf(text, 256, L"%f, %f", latitude, longitude);
}

ChatControlMessageLocation::~ChatControlMessageLocation()
{
}

int ChatControlMessageLocation::calculateHeight()
{
	int height = getPreviewBitmapHeight();

	HDC deviceContext = GetDC(NULL);
	height += calculateDrawTextHeight(deviceContext, text, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	ReleaseDC(NULL, deviceContext);

	if (getPreviewBitmapHeight() > 0)
	{
		height += 10;
	}

	return height;
}

void ChatControlMessageLocation::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y + 5);

	int yText = 0;

	if (getPreviewBitmapHeight() > 0)
	{
		yText += getPreviewBitmapHeight() + 10;
	}

	SetTextColor(deviceContext, RGB(0, 0, 0));
	drawText(deviceContext, text, x, y + yText, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
}
