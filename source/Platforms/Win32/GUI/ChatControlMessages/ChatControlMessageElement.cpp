#include <string>
#include <windows.h>

#include "ChatControlMessageElement.h"
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

void ChatControlMessageElement::calculateHeight(HDC deviceContext, int left, int right)
{
	if (type == CHAT_CONTROL_MESSAGE_ELEMENT_TEXT)
	{
		RECT textRect = { left, 0, right, 0 };
		HGDIOBJ oldFont = SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
		DrawText(deviceContext, wcharText, -1, &textRect, DT_CALCRECT | DT_WORDBREAK);
		SelectObject(deviceContext, oldFont);
		height = textRect.bottom - textRect.top;
	} else if (type == CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY)
	{
		height = 20;
	}
}

void ChatControlMessageElement::render(HDC deviceContext, int y, int left, int right, Smileys &smileys)
{
	if (type == CHAT_CONTROL_MESSAGE_ELEMENT_TEXT)
	{
		RECT rect = { left, y, right, y };
		DrawText(deviceContext, wcharText, -1, &rect, DT_CALCRECT | DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
		rect.right = right;
		DrawText(deviceContext, wcharText, -1, &rect, DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
	} else if (type == CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY)
	{
		smileys.renderSmiley(smiley, deviceContext, left, y);
	}
}

int ChatControlMessageElement::getHeight() const
{
	return height;
}
