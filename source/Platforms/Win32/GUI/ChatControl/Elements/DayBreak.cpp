#include <windows.h>
#include <string>

#include "DayBreak.h"
#include "../../DrawText.h"
#include "../../../Objects/Brush.h"
#include "../../../Objects/Font.h"
#include "../../../StringHelper.h"

DayBreak::DayBreak(const std::string &dayString, int width, Font &font)
	: width(width), font(font)
{
	dayStringW = strtowstr(dayString);
}

DayBreak::~DayBreak()
{
}

void DayBreak::updateWidth(HWND window, int width)
{
	this->width = width;
	calculateHeight();
}

void DayBreak::calculateHeight()
{
	HDC deviceContext = GetDC(NULL);
	height = calculateDrawTextHeight(deviceContext, dayStringW.c_str(), width, font.get());
	ReleaseDC(NULL, deviceContext);
}

int DayBreak::getHeight()
{
	return height;
}

void DayBreak::render(HDC deviceContext, int y, int clientHeight)
{
	SetTextColor(deviceContext, RGB(0, 0, 0));
	SetBkMode(deviceContext, TRANSPARENT);

	int textWidth = calculateDrawTextWidth(deviceContext, dayStringW.c_str(), font.get());

	int yCenter = y + height / 2;
	int xCenter = width / 2;
	MoveToEx(deviceContext, 10, yCenter, NULL);
	LineTo(deviceContext, xCenter - textWidth / 2 - 20, yCenter);
	MoveToEx(deviceContext, xCenter + textWidth / 2 + 20, yCenter, NULL);
	LineTo(deviceContext, width - 10, yCenter);

	drawText(deviceContext, dayStringW.c_str(), xCenter - textWidth / 2, y, textWidth);
}
