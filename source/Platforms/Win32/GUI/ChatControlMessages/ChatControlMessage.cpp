#include <windows.h>

#include "ChatControlMessage.h"
#include "../DrawText.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"
#include "../../../../WhatsApp/Message.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, int width, int color, HFONT dateFont)
	: message(message), width(width), color(color), dateFont(dateFont)
{
	wcharDate = strtowstr(formatTimestamp(message.getTimestamp()));
}

ChatControlMessage::~ChatControlMessage()
{
}

void ChatControlMessage::updateWidth(HWND window, int width)
{
	this->width = width;
	calculateHeight(window);
}

void ChatControlMessage::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderBox(deviceContext, x, y);
	renderInner(deviceContext, x, y, clientHeight);
}

void ChatControlMessage::renderBox(HDC deviceContext, int x, int y)
{
	SetBkColor(deviceContext, color);

	RECT completeRect = { x, y, x + width, y + getHeight() };
	HBRUSH brush = CreateSolidBrush(color);
	FillRect(deviceContext, &completeRect, brush);
	DeleteObject(brush);

	SetTextColor(deviceContext, RGB(110, 110, 110));
	int dateHeight = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), width, dateFont);
	drawTextRight(deviceContext, wcharDate.c_str(), x, y + getHeight() - dateHeight, width, dateFont);
}

int ChatControlMessage::getDateHeight(HDC deviceContext)
{
	return calculateDrawTextHeight(deviceContext, wcharDate.c_str(), width, dateFont);
}

WhatsappMessage &ChatControlMessage::getMessage()
{
	return message;
}
