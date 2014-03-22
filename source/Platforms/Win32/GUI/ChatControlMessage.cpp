#include <windows.h>

#include "ChatControlMessage.h"
#include "../../../WhatsApp/Message.h"
#include "../JpegDecoder.h"
#include "../StringHelper.h"
#include "../Timestamp.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, JpegDecoder &jpegDecoder) : message(message), bitmap(NULL)
{
	wcharText = buildWcharString(message.getData());
	wcharDate = buildTimestampString(message.getTimestamp());
	height = 0;

	if (message.getMediaWhatsappType() == IMAGE && message.getRawDataSize() > 0 && message.getRawData() != NULL)
	{
		bitmap = jpegDecoder.loadImage(message.getRawData(), message.getRawDataSize());
	}
}

ChatControlMessage::~ChatControlMessage()
{
	if (bitmap != NULL)
	{
		DeleteObject(bitmap);
	}

	delete[] wcharText;
	delete[] wcharDate;
}

WhatsappMessage &ChatControlMessage::getMessage()
{
	return message;
}

WCHAR *ChatControlMessage::getText()
{
	return wcharText;
}

WCHAR *ChatControlMessage::getDateText()
{
	return wcharDate;
}

int ChatControlMessage::getHeight()
{
	return height;
}

void ChatControlMessage::calculateHeight(HWND window, HGDIOBJ dateFont)
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	HDC deviceContext = GetDC(window);

	int gap = 40;
	int left = 10;
	int right = clientRect.right - gap - 10;
	height = 0;

	RECT textRect = { left, 0, right, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
	DrawText(deviceContext, wcharText, -1, &textRect, DT_CALCRECT | DT_WORDBREAK);
	SelectObject(deviceContext, oldFont);
	height += textRect.bottom - textRect.top;

	RECT dateRect = { left, height, right, height };
	oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_CALCRECT | DT_WORDBREAK | DT_RIGHT);
	dateRect.right = right;
	SelectObject(deviceContext, oldFont);
	height += dateRect.bottom - dateRect.top;

	if (message.getMediaWhatsappType() == IMAGE && bitmap != NULL)
	{
		BITMAP bitmapObject;
		GetObject(bitmap, sizeof(bitmapObject), &bitmapObject);
		height += bitmapObject.bmHeight;
	}

	ReleaseDC(window, deviceContext);
}

HBITMAP ChatControlMessage::getBitmap()
{
	return bitmap;
}
