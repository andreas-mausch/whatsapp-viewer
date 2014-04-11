#pragma once

class ChatControlMessageElement;
class JpegDecoder;
class WhatsappMessage;
class Smileys;

#include <vector>

class ChatControlMessage
{
private:
	std::vector<ChatControlMessageElement *> elements;
	WCHAR *wcharDate;
	int height;
	HBITMAP bitmap;

	WhatsappMessage &message;

	void splitMessage(WhatsappMessage &message);

public:
	ChatControlMessage(WhatsappMessage &message, JpegDecoder &jpegDecoder);
	~ChatControlMessage();

	void calculateHeight(HWND window, HGDIOBJ dateFont);

	WhatsappMessage &getMessage();
	WCHAR *getDateText();
	int getHeight();
	HBITMAP getBitmap();

	void render(HDC deviceContext, int y, int left, int right, Smileys &smileys);

};
