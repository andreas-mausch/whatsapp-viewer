#include <windows.h>
#include <string>

#include "ChatControlMessage.h"
#include "ChatControlMessageFrame.h"
#include "../DrawText.h"
#include "../../Objects/Brush.h"
#include "../../Objects/Font.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"
#include "../../../../WhatsApp/Message.h"

ChatControlMessageFrame::ChatControlMessageFrame(ChatControlMessage *message, int width, int color, Font &dateFont)
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

	if (message->getMessage().getRemoteResource().size() > 0)
	{
		height += getRemoteResourceHeight();
	}
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
	Brush brush(CreateSolidBrush(color));
	FillRect(deviceContext, &completeRect, brush.get());

	SetTextColor(deviceContext, RGB(110, 110, 110));
	int dateHeight = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), width, dateFont.get());
	drawTextRight(deviceContext, wcharDate.c_str(), x, y + getHeight() - dateHeight, width, dateFont.get());

	if (message->getMessage().getRemoteResource().size() > 0)
	{
		std::wstring remoteResource = strtowstr(message->getMessage().getRemoteResource());
		int remoteResourceHeight = calculateDrawTextHeight(deviceContext, remoteResource.c_str(), width, dateFont.get());
		drawTextRight(deviceContext, remoteResource.c_str(), x, y + getHeight() - dateHeight - remoteResourceHeight, width, dateFont.get());
	}
}

int ChatControlMessageFrame::getDateHeight()
{
	HDC deviceContext = GetDC(NULL);
	int height = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), width, dateFont.get());
	ReleaseDC(NULL, deviceContext);
	return height;
}

int ChatControlMessageFrame::getRemoteResourceHeight()
{
	HDC deviceContext = GetDC(NULL);
	int height = calculateDrawTextHeight(deviceContext, strtowstr(message->getMessage().getRemoteResource()).c_str(), width, dateFont.get());
	ReleaseDC(NULL, deviceContext);
	return height;
}
