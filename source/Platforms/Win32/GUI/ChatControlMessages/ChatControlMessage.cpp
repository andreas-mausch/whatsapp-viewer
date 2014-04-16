#include <windows.h>

#include "ChatControlMessage.h"
#include "../DrawText.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"
#include "../../../../WhatsApp/Message.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, HFONT dateFont)
	: message(message), dateFont(dateFont)
{
	wcharDate = strtowstr(formatTimestamp(message.getTimestamp()));
}

ChatControlMessage::~ChatControlMessage()
{
}

void ChatControlMessage::render(HDC deviceContext, int y, int left, int right, int clientHeight)
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
	renderInner(deviceContext, y, left, right, clientHeight);
}

void ChatControlMessage::renderBox(HDC deviceContext, int y, int left, int right, int color)
{
	SetBkColor(deviceContext, color);

	RECT completeRect = { left, y, right, y + getHeight() };
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(deviceContext, &completeRect, brush);
	DeleteObject(brush);

	SetTextColor(deviceContext, RGB(110, 110, 110));
	int dateHeight = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), right - left, dateFont);
	drawText(deviceContext, wcharDate.c_str(), left, y + getHeight() - dateHeight, right - left, dateFont);
}

bool ChatControlMessage::fromMe()
{
	return message.isFromMe();
}

int ChatControlMessage::getDateHeight(HDC deviceContext, int left, int right)
{
	return calculateDrawTextHeight(deviceContext, wcharDate.c_str(), right - left, dateFont);
}
