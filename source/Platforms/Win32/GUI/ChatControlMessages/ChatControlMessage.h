#pragma once

#include <string>

class WhatsappMessage;

class ChatControlMessage
{
private:
	WhatsappMessage &message;
	std::wstring wcharDate;
	int color;
	HFONT dateFont;

	void renderBox(HDC deviceContext, int x, int y);
	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight) = 0;

protected:
	int width;
	int getDateHeight(HDC deviceContext);

public:
	ChatControlMessage(WhatsappMessage &message, int width, int color, HFONT dateFont);
	virtual ~ChatControlMessage();

	virtual void calculateHeight(HWND window) = 0;
	virtual int getHeight() = 0;

	void updateWidth(HWND window, int width);
	void render(HDC deviceContext, int x, int y, int clientHeight);

	WhatsappMessage &getMessage();

};
