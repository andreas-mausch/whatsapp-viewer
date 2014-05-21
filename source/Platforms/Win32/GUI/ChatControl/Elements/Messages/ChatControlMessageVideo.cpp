#include <windows.h>

#include "ChatControlMessageVideo.h"
#include "../../../DrawText.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"

ChatControlMessageVideo::ChatControlMessageVideo(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, imageDecoder), text(L"Video")
{
}

ChatControlMessageVideo::~ChatControlMessageVideo()
{
}

int ChatControlMessageVideo::calculateHeight()
{
	int height = getPreviewBitmapHeight();

	HDC deviceContext = GetDC(NULL);
	height += calculateDrawTextHeight(deviceContext, text.c_str(), getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	ReleaseDC(NULL, deviceContext);

	height += 10;

	return height;
}

void ChatControlMessageVideo::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y + 5);

	SetTextColor(deviceContext, RGB(0, 0, 0));
	drawText(deviceContext, text.c_str(), x, y + getPreviewBitmapHeight() + 10, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
}
