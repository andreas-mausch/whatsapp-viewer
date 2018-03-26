#include <windows.h>
#include <string>

#include "ChatControlMessageFrame.h"
#include "Messages/ChatControlMessage.h"
#include "../../DrawText.h"
#include "../../../Objects/Brush.h"
#include "../../../Objects/Font.h"
#include "../../../StringHelper.h"
#include "../../../Timestamp.h"
#include "../../../../../WhatsApp/Message.h"

ChatControlMessageFrame::ChatControlMessageFrame(ChatControlMessage *message, int width, int color, Font &dateFont)
	: message(message), width(width), color(color), height(0), dateFont(dateFont)
{
	wcharDate = strtowstr(formatTimestamp(message->getMessage().getTimestamp()));
	wcharFilename = strtowstr(message->getMessage().getFilename());
}

ChatControlMessageFrame::~ChatControlMessageFrame()
{
	delete message;
}

void ChatControlMessageFrame::updateWidth(HWND window, int width)
{
	this->width = width;

	int gap = 40;
	int messageWidth = width - gap - 20;
	message->updateWidth(window, messageWidth);
	calculateHeight();
}

void ChatControlMessageFrame::calculateHeight()
{
	height = message->calculateHeight();
	height += getHeight(wcharFilename);
	height += getHeight(wcharDate);

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

void ChatControlMessageFrame::render(HDC deviceContext, int y, int clientHeight)
{
	int x = 10;
	if (message->getMessage().isFromMe())
	{
		x += 40;
	}

	renderFrame(deviceContext, x, y);
	message->render(deviceContext, x, y, clientHeight);
}

void ChatControlMessageFrame::renderFrame(HDC deviceContext, int x, int y)
{
	int gap = 40;
	int messageWidth = width - gap - 20;

	SetBkColor(deviceContext, color);

	RECT completeRect = { x, y, x + messageWidth, y + getHeight() };
	Brush brush(CreateSolidBrush(color));
	FillRect(deviceContext, &completeRect, brush.get());

	SetTextColor(deviceContext, RGB(110, 110, 110));
	int filenameHeight = calculateDrawTextHeight(deviceContext, wcharFilename.c_str(), messageWidth, dateFont.get());
	int dateHeight = calculateDrawTextHeight(deviceContext, wcharDate.c_str(), messageWidth, dateFont.get());

	drawTextRight(deviceContext, wcharFilename.c_str(), x, y + getHeight() - dateHeight - filenameHeight, messageWidth, dateFont.get());
	drawTextRight(deviceContext, wcharDate.c_str(), x, y + getHeight() - dateHeight, messageWidth, dateFont.get());

	if (message->getMessage().getRemoteResource().size() > 0)
	{
		std::wstring remoteResource = strtowstr(message->getMessage().getRemoteResource());
		int remoteResourceHeight = calculateDrawTextHeight(deviceContext, remoteResource.c_str(), messageWidth, dateFont.get());
		drawTextRight(deviceContext, remoteResource.c_str(), x, y + getHeight() - dateHeight - filenameHeight - remoteResourceHeight, messageWidth, dateFont.get());
	}
}

int ChatControlMessageFrame::getHeight(const std::wstring &string)
{
	HDC deviceContext = GetDC(NULL);
	int height = calculateDrawTextHeight(deviceContext, string.c_str(), width, dateFont.get());
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
