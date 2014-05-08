#include <windows.h>

#include "Smiley.h"
#include "../Objects/Bitmap.h"

// TODO: merge with ChatControl.cpp
void renderBitmap2(HDC deviceContext, HBITMAP bitmapHandle, int x, int y)
{
	BITMAP bitmap;
	HDC hdcMem = CreateCompatibleDC(deviceContext);
	HGDIOBJ oldBitmap = SelectObject(hdcMem, bitmapHandle);

	GetObject(bitmapHandle, sizeof(bitmap), &bitmap);
	BLENDFUNCTION blend;
	memset(&blend, 0, sizeof(BLENDFUNCTION));
	blend.BlendOp = AC_SRC_OVER;
	blend.SourceConstantAlpha = 255;
	blend.AlphaFormat = AC_SRC_ALPHA;

	AlphaBlend(deviceContext, x, y, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, bitmap.bmWidth, bitmap.bmHeight, blend);

	SelectObject(hdcMem, oldBitmap);
	DeleteDC(hdcMem);
}

Smiley::Smiley(int character, HBITMAP bitmap) : character(character), bitmap(bitmap)
{
}

Smiley::~Smiley()
{
}

void Smiley::render(HDC deviceContext, int x, int y)
{
	renderBitmap2(deviceContext, bitmap.get(), x, y);
}

int Smiley::getCharacter() const
{
	return character;
}
