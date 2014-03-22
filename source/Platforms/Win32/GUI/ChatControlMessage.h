#pragma once

class JpegDecoder;
class WhatsappMessage;

class ChatControlMessage
{
private:
	WCHAR *wcharText;
	WCHAR *wcharDate;
	int height;
	HBITMAP bitmap;

	WhatsappMessage &message;

public:
	ChatControlMessage(WhatsappMessage &message, JpegDecoder &jpegDecoder);
	~ChatControlMessage();

	void calculateHeight(HWND window, HGDIOBJ dateFont);

	WhatsappMessage &getMessage();
	WCHAR *getText();
	WCHAR *getDateText();
	int getHeight();
	HBITMAP getBitmap();
};
