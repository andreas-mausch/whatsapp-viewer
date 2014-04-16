#include <windows.h>

#include "ChatControlMessageText.h"
#include "ChatControlMessageElement.h"
#include "../SmileyList.h"
#include "../../../../WhatsApp/Message.h"
#include "../../../../UTF8/utf8.h"
#include "../../../../VectorUtils.h"
#include "../../StringHelper.h"
#include "../../Timestamp.h"

ChatControlMessageText::ChatControlMessageText(WhatsappMessage &message, int width, int color, HFONT dateFont, Smileys &smileys)
	: ChatControlMessage(message, width, color, dateFont), smileys(smileys), height(0)
{
	splitMessage(message);
}

ChatControlMessageText::~ChatControlMessageText()
{
	clearVector(elements);
}

int ChatControlMessageText::getHeight()
{
	return height;
}

void ChatControlMessageText::calculateHeight(HWND window)
{
	HDC deviceContext = GetDC(window);

	height = 0;

	for (std::vector<ChatControlMessageElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ChatControlMessageElement &element = **it;
		element.calculateHeight(deviceContext, width);
		height += element.getHeight();
	}

	height += getDateHeight(deviceContext);

	ReleaseDC(window, deviceContext);
}

bool ChatControlMessageText::isSmiley(int character)
{
	for (int i = 0; i < smileyCount; i++)
	{
		if (smileyList[i].character == character)
		{
			return true;
		}
	}

	return false;
}

void ChatControlMessageText::splitMessage(WhatsappMessage &message)
{
	std::string messageString = message.getData();
	int lastSplit = 0;
	for (std::string::iterator it = messageString.begin(); it != messageString.end();)
	{
		bool begin = (it - messageString.begin()) == lastSplit;
		std::string::iterator before = it;
		int character = utf8::next(it, messageString.end());

		if (isSmiley(character))
		{
			if (!begin)
			{
				int start = lastSplit;
				int end = before - messageString.begin();
				std::string leftPart = messageString.substr(start, end - start);
				elements.push_back(new ChatControlMessageElement(leftPart));
			}

			elements.push_back(new ChatControlMessageElement(character));

			lastSplit = (it - messageString.begin());
		}
	}

	int length = messageString.length();
	if (lastSplit < length - 1)
	{
		std::string part = messageString.substr(lastSplit, length);
		elements.push_back(new ChatControlMessageElement(part));
	}
}

void ChatControlMessageText::renderInner(HDC deviceContext, int x, int y, int clientHeight)
{
	SetTextColor(deviceContext, RGB(0, 0, 0));

	for (std::vector<ChatControlMessageElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ChatControlMessageElement &element = **it;

		if (y + element.getHeight() > 0)
		{
			element.render(deviceContext, x, y, width, smileys);
		}
		y += element.getHeight();

		if (y > clientHeight)
		{
			break;
		}
	}
}
