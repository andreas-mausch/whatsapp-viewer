#include <windows.h>

#include "ChatControlMessageElementThumbnail.h"
#include "../../../DrawText.h"
#include "../../../../ImageDecoder.h"
#include "../../../../StringHelper.h"
#include "../../../../Objects/Bitmap.h"
#include "../../../../../../WhatsApp/Message.h"

ChatControlMessageElementThumbnail::ChatControlMessageElementThumbnail(unsigned char *data, int size, ImageDecoder &imageDecoder)
	: thumbnail(NULL), thumbnailWidth(0), thumbnailHeight(0)
{
	if (data != NULL && size > 0)
	{
		HBITMAP bitmap = imageDecoder.loadImage(data, size);

		BITMAP bitmapObject;
		GetObject(bitmap, sizeof(BITMAP), &bitmapObject);
		thumbnailWidth = bitmapObject.bmWidth;
		thumbnailHeight = bitmapObject.bmHeight;

		this->thumbnail = new Bitmap(bitmap);
	}
}

ChatControlMessageElementThumbnail::~ChatControlMessageElementThumbnail()
{
	delete thumbnail;
}

void ChatControlMessageElementThumbnail::calculateHeight(int width)
{
	height = thumbnailHeight + 10;
}

void ChatControlMessageElementThumbnail::render(HDC deviceContext, int x, int y, int clientHeight, Smileys &smileys)
{
	renderThumbnail(deviceContext, x + 5, y + 5);
}

void ChatControlMessageElementThumbnail::renderThumbnail(HDC deviceContext, int x, int y)
{
	if (thumbnail != NULL)
	{
		HDC hdcMem = CreateCompatibleDC(deviceContext);
		HGDIOBJ oldBitmap = SelectObject(hdcMem, thumbnail->get());

		BitBlt(deviceContext, x, y, thumbnailWidth, thumbnailHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
	}
}
