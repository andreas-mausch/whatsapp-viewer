#pragma once

class ChatControlElement
{
public:
	ChatControlElement() { }
	virtual ~ChatControlElement() { }

	virtual void updateWidth(HWND window, int width) = 0;
	virtual void calculateHeight() = 0;

	virtual int getHeight() = 0;

	virtual void render(HDC deviceContext, int y, int clientHeight) = 0;

};
