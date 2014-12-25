#include <sstream>
#include <windows.h>

#include "ChatControlMessageText.h"
#include "ChatControlMessageTextElement.h"
#include "../../../SmileyList.h"
#include "../../../../../../Libraries/UTF8/utf8.h"
#include "../../../../../../WhatsApp/Emoticons.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../../../VectorUtils.h"
#include "../../../../StringHelper.h"
#include "../../../../Timestamp.h"

ChatControlMessageText::ChatControlMessageText(WhatsappMessage &message, int width, Smileys &smileys)
	: ChatControlMessage(message, width), smileys(smileys)
{
	splitMessage(message);
}

ChatControlMessageText::~ChatControlMessageText()
{
	clearVector(elements);
}

int ChatControlMessageText::calculateHeight()
{
	int height = 0;

	for (std::vector<ChatControlMessageTextElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ChatControlMessageTextElement &element = **it;
		element.calculateHeight(getWidth());
		height += element.getHeight();
	}

	return height;
}

void ChatControlMessageText::splitMessage(WhatsappMessage &message)
{
	std::string messageString = message.getData();

	try
	{
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
					elements.push_back(new ChatControlMessageTextElement(leftPart));
				}

				elements.push_back(new ChatControlMessageTextElement(character));

				lastSplit = (it - messageString.begin());
			}
		}

		int length = messageString.length();
		if (lastSplit < length)
		{
			std::string part = messageString.substr(lastSplit, length);
			elements.push_back(new ChatControlMessageTextElement(part));
		}
	}
	catch (utf8::exception &exception)
	{
		std::stringstream invalidDataString;
		invalidDataString << "[INVALID DATA: " << messageString << "]";
		elements.push_back(new ChatControlMessageTextElement(invalidDataString.str()));
	}
}

void ChatControlMessageText::render(HDC deviceContext, int x, int y, int clientHeight)
{
	SetTextColor(deviceContext, RGB(0, 0, 0));

	for (std::vector<ChatControlMessageTextElement *>::iterator it = elements.begin(); it != elements.end(); it++)
	{
		ChatControlMessageTextElement &element = **it;

		if (y + element.getHeight() > 0)
		{
			element.render(deviceContext, x, y, getWidth(), smileys);
		}
		y += element.getHeight();

		if (y > clientHeight)
		{
			break;
		}
	}
}
