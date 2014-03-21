#include <windows.h>

#include "ChatControlMessage.h"
#include "../../../WhatsApp/Message.h"
#include "../StringHelper.h"
#include "../Timestamp.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message) : message(message)
{
	wcharText = buildWcharString(message.getData());
	wcharDate = buildTimestampString(message.getTimestamp());
	height = 0;
}

ChatControlMessage::~ChatControlMessage()
{
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
	DrawText(deviceContext, wcharText, -1, &textRect, DT_CALCRECT | DT_WORDBREAK);
	height += textRect.bottom - textRect.top;

	RECT dateRect = { left, height, right, height };
	HGDIOBJ oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_CALCRECT | DT_WORDBREAK | DT_RIGHT);
	dateRect.right = right;
	SelectObject(deviceContext, oldFont);
	height += dateRect.bottom - dateRect.top;

	ReleaseDC(window, deviceContext);
}
