#include <windows.h>

#include "ChatControlMessage.h"
#include "ChatControlMessageElement.h"
#include "../../../WhatsApp/Message.h"
#include "../../../UTF8/utf8.h"
#include "../JpegDecoder.h"
#include "../StringHelper.h"
#include "../Timestamp.h"

ChatControlMessage::ChatControlMessage(WhatsappMessage &message, JpegDecoder &jpegDecoder) : message(message), bitmap(NULL)
{
	splitMessage(message);
	wcharDate = buildTimestampString(message.getTimestamp());
	height = 0;

	if (message.getMediaWhatsappType() == MEDIA_WHATSAPP_IMAGE && message.getRawDataSize() > 0 && message.getRawData() != NULL)
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
	for (std::vector<ChatControlMessageElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		delete *it;
	}

	delete[] wcharDate;
}

WhatsappMessage &ChatControlMessage::getMessage()
{
	return message;
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

	for (std::vector<ChatControlMessageElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ChatControlMessageElement &element = **it;
		element.calculateHeight(deviceContext, left, right);
		height += element.getHeight();
	}

	RECT dateRect = { left, height, right, height };
	HGDIOBJ oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_CALCRECT | DT_WORDBREAK | DT_RIGHT);
	dateRect.right = right;
	SelectObject(deviceContext, oldFont);
	height += dateRect.bottom - dateRect.top;

	if (message.getMediaWhatsappType() == MEDIA_WHATSAPP_IMAGE && bitmap != NULL)
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

void ChatControlMessage::splitMessage(WhatsappMessage &message)
{
	std::string messageString = message.getData();
	int lastSplit = 0;
	for (std::string::iterator it = messageString.begin(); it != messageString.end();)
	{
		bool begin = (it - messageString.begin()) == lastSplit;
		int character = utf8::next(it, messageString.end());

		if (character == 0x1F61F || character == 0x0e418)
		{
			if (!begin)
			{
				int end = it - messageString.begin();
				std::string leftPart = messageString.substr(lastSplit, end);
				elements.push_back(new ChatControlMessageElement(leftPart));
			}

			elements.push_back(new ChatControlMessageElement(character));

			lastSplit = (it - messageString.begin());
		}
	}

	if (lastSplit < messageString.length() - 1)
	{
		std::string part = messageString.substr(lastSplit, messageString.length());
		elements.push_back(new ChatControlMessageElement(part));
	}
}

void ChatControlMessage::render(HDC deviceContext, int y, int left, int right, Smileys &smileys)
{
	for (std::vector<ChatControlMessageElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ChatControlMessageElement &element = **it;
		element.render(deviceContext, y, left, right, smileys);
		y += element.getHeight();
	}
}
