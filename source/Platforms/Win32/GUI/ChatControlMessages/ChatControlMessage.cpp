#include <windows.h>

#include "ChatControlMessage.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"
#include "../../../../WhatsApp/Message.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, HGDIOBJ dateFont)
	: message(message), dateFont(dateFont)
{
	wcharDate = buildTimestampString(message.getTimestamp());
}

ChatControlMessage::~ChatControlMessage()
{
	delete[] wcharDate;
}

void ChatControlMessage::render(HDC deviceContext, int y, int left, int right)
{
	int gap = 40;
	int color;

	if (fromMe())
	{
		left += gap;
		color = RGB(190, 240, 150);
	}
	else
	{
		right -= gap;
		color = RGB(230, 230, 240);
	}

	renderBox(deviceContext, y, left, right, color);
	renderInner(deviceContext, y, left, right);
}

void ChatControlMessage::renderBox(HDC deviceContext, int y, int left, int right, int color)
{

	SetBkColor(deviceContext, color);
	RECT textRect = { left, y, right, y };

	RECT dateRect = { left, 0, right, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_CALCRECT | DT_WORDBREAK | DT_RIGHT);
	dateRect.right = right;
	dateRect.top = y + getHeight() - dateRect.bottom;
	dateRect.bottom = y + getHeight();
	SelectObject(deviceContext, oldFont);

	RECT completeRect = { left, y, right, y + getHeight() };

	HBRUSH brush = CreateSolidBrush(color);
	FillRect(deviceContext, &completeRect, brush);
	DeleteObject(brush);

	oldFont = SelectObject(deviceContext, dateFont);
	SetTextColor(deviceContext, RGB(110, 110, 110));
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_WORDBREAK | DT_RIGHT);
	SelectObject(deviceContext, oldFont);
}

bool ChatControlMessage::fromMe()
{
	return message.isFromMe();
}

int ChatControlMessage::getDateHeight(HDC deviceContext, int left, int right)
{
	RECT dateRect = { left, 0, right, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_CALCRECT | DT_WORDBREAK | DT_RIGHT);
	dateRect.right = right;
	SelectObject(deviceContext, oldFont);
	return dateRect.bottom - dateRect.top;
}
