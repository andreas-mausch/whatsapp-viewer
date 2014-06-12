#include <windows.h>

#include "DrawText.h"

void calculateDrawTextRect(HDC deviceContext, const WCHAR *text, RECT &rect, int flags)
{
	int right = rect.right;
	DrawText(deviceContext, text, -1, &rect, DT_CALCRECT | DT_NOPREFIX | flags);
	rect.right = right;
}

void calculateDrawTextRect(HDC deviceContext, const WCHAR *text, RECT &rect)
{
	calculateDrawTextRect(deviceContext, text, rect, DT_WORDBREAK);
}

int calculateDrawTextHeight(HDC deviceContext, const WCHAR *text, int width, HFONT font)
{
	RECT rect = { 0, 0, width, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, font);
	calculateDrawTextRect(deviceContext, text, rect);
	SelectObject(deviceContext, oldFont);
	return rect.bottom - rect.top;
}

int calculateDrawTextWidth(HDC deviceContext, const WCHAR *text, HFONT font)
{
	RECT rect = { 0, 0, 0, 0 };
	HGDIOBJ oldFont = SelectObject(deviceContext, font);
	DrawText(deviceContext, text, -1, &rect, DT_CALCRECT | DT_NOPREFIX);
	SelectObject(deviceContext, oldFont);
	return rect.right - rect.left;
}

void drawText(HDC deviceContext, const WCHAR *text, const RECT &rect, int flags)
{
	RECT localRect = rect;
	DrawText(deviceContext, text, -1, &localRect, flags);
}

void drawText(HDC deviceContext, const WCHAR *text, const RECT &rect)
{
	drawText(deviceContext, text, rect, DT_WORDBREAK | DT_NOPREFIX);
}

void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width)
{
	RECT rect = { x, y, x + width, y };
	calculateDrawTextRect(deviceContext, text, rect);
	drawText(deviceContext, text, rect);
}

void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width, int flags)
{
	RECT rect = { x, y, x + width, y };
	calculateDrawTextRect(deviceContext, text, rect, flags);
	drawText(deviceContext, text, rect, flags);
}

void drawText(HDC deviceContext, const WCHAR *text, int x, int y, int width, HFONT font)
{
	HGDIOBJ oldFont = SelectObject(deviceContext, font);
	drawText(deviceContext, text, x, y, width);
	SelectObject(deviceContext, oldFont);
}

void drawTextRight(HDC deviceContext, const WCHAR *text, int x, int y, int width, HFONT font)
{
	HGDIOBJ oldFont = SelectObject(deviceContext, font);
	drawText(deviceContext, text, x, y, width, DT_WORDBREAK | DT_RIGHT);
	SelectObject(deviceContext, oldFont);
}

void drawTextCentered(HDC deviceContext, const WCHAR *text, int x, int y, int width)
{
	drawText(deviceContext, text, x, y, width, DT_WORDBREAK | DT_CENTER);
}
