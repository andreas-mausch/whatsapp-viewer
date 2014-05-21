#include <windows.h>

#include "ChatControlMessageWithPreview.h"
#include "../../../../../../WhatsApp/Message.h"
#include "../../../../ImageDecoder.h"
#include "../../../../Objects/Bitmap.h"

ChatControlMessageWithPreview::ChatControlMessageWithPreview(WhatsappMessage &message, int width, ImageDecoder &imageDecoder)
	: ChatControlMessage(message, width), bitmap(NULL), bitmapWidth(0), bitmapHeight(0)
{
	if (message.getRawData() != NULL && message.getRawDataSize() > 0)
	{
		HBITMAP bitmap = imageDecoder.loadImage(message.getRawData(), message.getRawDataSize());

		BITMAP bitmapObject;
		GetObject(bitmap, sizeof(BITMAP), &bitmapObject);
		bitmapWidth = bitmapObject.bmWidth;
		bitmapHeight = bitmapObject.bmHeight;

		this->bitmap = new Bitmap(bitmap);
	}
}

ChatControlMessageWithPreview::~ChatControlMessageWithPreview()
{
	delete bitmap;
}

int ChatControlMessageWithPreview::getPreviewBitmapWidth()
{
	return bitmapWidth;
}

int ChatControlMessageWithPreview::getPreviewBitmapHeight()
{
	return bitmapHeight;
}

void ChatControlMessageWithPreview::renderPreviewBitmap(HDC deviceContext, int x, int y)
{
	if (bitmap != NULL)
	{
		HDC hdcMem = CreateCompatibleDC(deviceContext);
		HGDIOBJ oldBitmap = SelectObject(hdcMem, bitmap->get());

		BitBlt(deviceContext, x, y, bitmapWidth, bitmapHeight, hdcMem, 0, 0, SRCCOPY);

		SelectObject(hdcMem, oldBitmap);
		DeleteDC(hdcMem);
	}
}
