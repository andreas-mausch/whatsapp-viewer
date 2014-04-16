#pragma once

#include <string>

class WhatsappMessage;

class ChatControlMessage
{
private:
	WhatsappMessage &message;
	std::wstring wcharDate;
	int color;
	int width;
	int height;
	HFONT dateFont;

	void renderBox(HDC deviceContext, int x, int y);
	int getDateHeight();

	virtual int calculateHeightInner() = 0;
	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight) = 0;

public:
	ChatControlMessage(WhatsappMessage &message, int width, int color, HFONT dateFont);
	virtual ~ChatControlMessage();

	int getWidth();
	int getHeight();
	void updateWidth(HWND window, int width);
	void calculateHeight();
	void render(HDC deviceContext, int x, int y, int clientHeight);

	WhatsappMessage &getMessage();

};
