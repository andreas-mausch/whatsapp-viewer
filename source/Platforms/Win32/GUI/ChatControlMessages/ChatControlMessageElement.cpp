#include <string>
#include <windows.h>

#include "ChatControlMessageElement.h"
#include "../DrawText.h"
#include "../Smileys.h"
#include "../../StringHelper.h"
#include "../../../../Exceptions/Exception.h"

ChatControlMessageElement::ChatControlMessageElement(const std::string &text)
{
	type = CHAT_CONTROL_MESSAGE_ELEMENT_TEXT;
	this->text = text;
	wcharText = buildWcharString(text);
	height = 0;
}

ChatControlMessageElement::ChatControlMessageElement(int smiley)
{
	type = CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY;
	this->smiley = smiley;
	wcharText = NULL;
	height = 0;
}

ChatControlMessageElement::~ChatControlMessageElement()
{
	delete[] wcharText;
}

void ChatControlMessageElement::calculateHeight(HDC deviceContext, int width)
{
	if (type == CHAT_CONTROL_MESSAGE_ELEMENT_TEXT)
	{
		height = calculateDrawTextHeight(deviceContext, wcharText, width, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	} else if (type == CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY)
	{
		height = 20;
	}
}

void ChatControlMessageElement::render(HDC deviceContext, int x, int y, int width, Smileys &smileys)
{
	if (type == CHAT_CONTROL_MESSAGE_ELEMENT_TEXT)
	{
		drawText(deviceContext, wcharText, x, y, width);
	} else if (type == CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY)
	{
		smileys.renderSmiley(smiley, deviceContext, x, y);
	}
}

int ChatControlMessageElement::getHeight() const
{
	return height;
}
