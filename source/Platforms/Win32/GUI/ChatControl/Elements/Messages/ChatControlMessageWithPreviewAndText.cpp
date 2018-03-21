#include <windows.h>

#include "ChatControlMessageWithPreviewAndText.h"
#include "../../../DrawText.h"
#include "../../../../ImageDecoder.h"
#include "../../../../StringHelper.h"
#include "../../../../Objects/Bitmap.h"
#include "../../../../../../WhatsApp/Message.h"

ChatControlMessageWithPreviewAndText::ChatControlMessageWithPreviewAndText(WhatsappMessage &message, unsigned char *data, int size, const std::string &text, int width, ImageDecoder &imageDecoder)
	: ChatControlMessage(message, width), text(text), thumbnail(NULL), thumbnailWidth(0), thumbnailHeight(0)
{
	if (text.length() > 0)
	{
		wcharText = buildWcharString(text);
	}

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

ChatControlMessageWithPreviewAndText::~ChatControlMessageWithPreviewAndText()
{
	if (text.length() > 0)
	{
		delete[] wcharText;
	}
	delete thumbnail;
}

int ChatControlMessageWithPreviewAndText::calculateHeight()
{
	int height = getThumbnailHeight();

	if (text.length() > 0)
	{
		HDC deviceContext = GetDC(NULL);
		height += calculateDrawTextHeight(deviceContext, wcharText, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
		ReleaseDC(NULL, deviceContext);

		if (getThumbnailHeight() > 0)
		{
			height += 10;
		}
	}

	return height;
}

void ChatControlMessageWithPreviewAndText::render(HDC deviceContext, int x, int y, int clientHeight)
{
	renderThumbnail(deviceContext, x + 5, y + 5);

	if (text.length() > 0)
	{
		int yText = 0;

		if (getThumbnailHeight() > 0)
		{
			yText += getThumbnailHeight() + 10;
		}

		SetTextColor(deviceContext, RGB(0, 0, 0));
		drawText(deviceContext, wcharText, x, y + yText, getWidth(), static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
	}
}

int ChatControlMessageWithPreviewAndText::getThumbnailWidth()
{
	return thumbnailWidth;
}

int ChatControlMessageWithPreviewAndText::getThumbnailHeight()
{
	return thumbnailHeight;
}

void ChatControlMessageWithPreviewAndText::renderThumbnail(HDC deviceContext, int x, int y)
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
