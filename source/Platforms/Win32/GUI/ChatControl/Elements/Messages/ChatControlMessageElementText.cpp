#include <string>
#include <windows.h>

#include "ChatControlMessageElementText.h"
#include "../../../DrawText.h"
#include "../../../../StringHelper.h"
#include "../../../../../../Exceptions/Exception.h"

ChatControlMessageElementText::ChatControlMessageElementText(const std::string &text)
	: text(text)
{
	wcharText = buildWcharString(text);
}

ChatControlMessageElementText::~ChatControlMessageElementText()
{
	delete[] wcharText;
}

void ChatControlMessageElementText::calculateHeight(int width)
{
	HDC deviceContext = GetDC(NULL);
	height = calculateDrawTextHeight(deviceContext, wcharText, width, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	ReleaseDC(NULL, deviceContext);
}

void ChatControlMessageElementText::render(HDC deviceContext, int x, int y, int width, Smileys &smileys)
{
	drawText(deviceContext, wcharText, x, y, width);
}
