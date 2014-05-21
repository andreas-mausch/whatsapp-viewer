#pragma once

#include <string>

class WhatsappMessage;

class ChatControlMessage
{
private:
	WhatsappMessage &message;
	int width;

public:
	ChatControlMessage(WhatsappMessage &message, int width);
	virtual ~ChatControlMessage();

	int getWidth();
	void updateWidth(HWND window, int width);

	virtual int calculateHeight() = 0;
	virtual void render(HDC deviceContext, int x, int y, int clientHeight) = 0;

	WhatsappMessage &getMessage();

};
