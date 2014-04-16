#include <windows.h>
#include <string>

#include "ChatControlMessage.h"
#include "ChatControlMessageFrame.h"
#include "../DrawText.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"
#include "../../../../WhatsApp/Message.h"

ChatControlMessageFrame::ChatControlMessageFrame(ChatControlMessage *message, int width, int color, HFONT dateFont)
	: message(message), width(width), color(color), height(0), dateFont(dateFont)
{
	wcharDate = strtowstr(formatTimestamp(message->getMessage().getTimestamp()));
}

ChatControlMessageFrame::~ChatControlMessageFrame()
{
	delete message;
}

void ChatControlMessageFrame::updateWidth(HWND window, int width)
{
	this->width = width;
	message->updateWidth(window, width);
	calculateHeight();
}

void ChatControlMessageFrame::calculateHeight()
{
	height = message->calculateHeight();
	height += getDateHeight();
}

ChatControlMessage *ChatControlMessageFrame::getMessage()
{
	return message;
}

int ChatControlMessageFrame::getHeight()
{
	return height;
}

void ChatControlMessageFrame::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderFrame(deviceContext, x, y);
	message->render(deviceContext, x, y, clientHeight);
}

void ChatControlMessageFrame::renderFrame(HDC deviceContext, int x, int y)
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

int ChatControlMessageFrame::getDateHeight()
{
	HDC deviceContext = GetDC(NULL);
	int height = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), width, dateFont);
	ReleaseDC(NULL, deviceContext);
	return height;
}
