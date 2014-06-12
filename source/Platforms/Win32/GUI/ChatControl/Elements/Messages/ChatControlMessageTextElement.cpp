#include <string>
#include <windows.h>

#include "ChatControlMessageTextElement.h"
#include "../../../DrawText.h"
#include "../../../Smileys.h"
#include "../../../../StringHelper.h"
#include "../../../../../../Exceptions/Exception.h"

ChatControlMessageTextElement::ChatControlMessageTextElement(const std::string &text)
{
	type = CHAT_CONTROL_MESSAGE_ELEMENT_TEXT;
	this->text = text;
	wcharText = buildWcharString(text);
	height = 0;
}

ChatControlMessageTextElement::ChatControlMessageTextElement(int smiley)
{
	type = CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY;
	this->smiley = smiley;
	wcharText = NULL;
	height = 0;
}

ChatControlMessageTextElement::~ChatControlMessageTextElement()
{
	delete[] wcharText;
}

void ChatControlMessageTextElement::calculateHeight(int width)
{
	if (type == CHAT_CONTROL_MESSAGE_ELEMENT_TEXT)
	{
		HDC deviceContext = GetDC(NULL);
		height = calculateDrawTextHeight(deviceContext, wcharText, width, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
		ReleaseDC(NULL, deviceContext);
	} else if (type == CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY)
	{
		height = 20;
	}
}

void ChatControlMessageTextElement::render(HDC deviceContext, int x, int y, int width, Smileys &smileys)
{
	if (type == CHAT_CONTROL_MESSAGE_ELEMENT_TEXT)
	{
		drawText(deviceContext, wcharText, x, y, width);
	} else if (type == CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY)
	{
		smileys.renderSmiley(smiley, deviceContext, x, y);
	}
}

int ChatControlMessageTextElement::getHeight() const
{
	return height;
}
