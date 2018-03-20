#pragma once

#include "ChatControlMessageWithPreview.h"

class ImageDecoder;
class WhatsappMessage;

class ChatControlMessageWithPreviewAndText : public ChatControlMessageWithPreview
{
private:
	std::string text;
	WCHAR *wcharText;

public:
	ChatControlMessageWithPreviewAndText(WhatsappMessage &message, const std::string &text, int width, ImageDecoder &imageDecoder);
	virtual ~ChatControlMessageWithPreviewAndText();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
