#include <windows.h>

#include "ChatControlMessage.h"
#include "../DrawText.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"
#include "../../../../WhatsApp/Message.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, int width, int color, HFONT dateFont)
	: message(message), width(width), height(0), color(color), dateFont(dateFont)
{
	wcharDate = strtowstr(formatTimestamp(message.getTimestamp()));
}

ChatControlMessage::~ChatControlMessage()
{
}

void ChatControlMessage::updateWidth(HWND window, int width)
{
	this->width = width;
	calculateHeight();
}

void ChatControlMessage::calculateHeight()
{
	height = calculateHeightInner();
	height += getDateHeight();
}

int ChatControlMessage::getWidth()
{
	return width;
}

int ChatControlMessage::getHeight()
{
	return height;
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

int ChatControlMessage::getDateHeight()
{
	HDC deviceContext = GetDC(NULL);
	int height = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), width, dateFont);
	ReleaseDC(NULL, deviceContext);
	return height;
}

WhatsappMessage &ChatControlMessage::getMessage()
{
	return message;
}
