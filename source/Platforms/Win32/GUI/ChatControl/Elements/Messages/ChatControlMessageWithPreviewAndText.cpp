#include <windows.h>

#include "ChatControlMessageWithPreviewAndText.h"
#include "../../../DrawText.h"
#include "../../../../ImageDecoder.h"
#include "../../../../StringHelper.h"
#include "../../../../../../WhatsApp/Message.h"

ChatControlMessageWithPreviewAndText::ChatControlMessageWithPreviewAndText(WhatsappMessage &message, const std::string &text, int width, ImageDecoder &imageDecoder)
	: ChatControlMessageWithPreview(message, width, imageDecoder)
{
	wcharText = buildWcharString(text);
}

ChatControlMessageWithPreviewAndText::~ChatControlMessageWithPreviewAndText()
{
	delete[] wcharText;
}

int ChatControlMessageWithPreviewAndText::calculateHeight()
{
	int height = getPreviewBitmapHeight();

	HDC deviceContext = GetDC(NULL);
	height += calculateDrawTextHeight(deviceContext, wcharText, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	ReleaseDC(NULL, deviceContext);

	if (getPreviewBitmapHeight() > 0)
	{
		height += 10;
	}

	return height;
}

void ChatControlMessageWithPreviewAndText::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderPreviewBitmap(deviceContext, x + 5, y + 5);

	int yText = 0;

	if (getPreviewBitmapHeight() > 0)
	{
		yText += getPreviewBitmapHeight() + 10;
	}

	SetTextColor(deviceContext, RGB(0, 0, 0));
	drawText(deviceContext, wcharText, x, y + yText, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
}
