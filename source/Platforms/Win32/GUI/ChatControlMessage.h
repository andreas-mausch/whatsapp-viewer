#pragma once

class WhatsappMessage;

class ChatControlMessage
{
private:
	WCHAR *wcharText;
	WCHAR *wcharDate;
	int height;

	WhatsappMessage &message;

public:
	ChatControlMessage(WhatsappMessage &message);
	~ChatControlMessage();

	void calculateHeight(HWND window, HGDIOBJ dateFont);

	WhatsappMessage &getMessage();
	WCHAR *getText();
	WCHAR *getDateText();
	int getHeight();
};
