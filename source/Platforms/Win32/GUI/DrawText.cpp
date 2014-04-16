#include <windows.h>

#include "DrawText.h"

void calculateDrawTextRect(HDC deviceContext, const WCHAR *text, RECT &rect)
{
	DrawText(deviceContext, text, -1, &rect, DT_CALCRECT | DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
}

int calculateDrawTextHeight(HDC deviceContext, const WCHAR *text, int width, HFONT font)
{
	RECT rect = { 0, 0, width, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, font);
	calculateDrawTextRect(deviceContext, text, rect);
	SelectObject(deviceContext, oldFont);
	return rect.bottom - rect.top;
}

void drawText(HDC deviceContext, const WCHAR *text, const RECT &rect)
{
	RECT localRect = rect;
	DrawText(deviceContext, text, -1, &localRect, DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
}

void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width)
{
	RECT rect = { x, y, x + width, y };
	calculateDrawTextRect(deviceContext, text, rect);
	drawText(deviceContext, text, rect);
}

void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width, HFONT font)
{
	HGDIOBJ oldFont = SelectObject(deviceContext, font);
	drawText(deviceContext, text, x, y, width);
	SelectObject(deviceContext, oldFont);
}
