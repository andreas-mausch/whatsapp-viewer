#pragma once

#include <string>
#include "../ChatControlElement.h"

class ChatControlMessage;
class Font;

class ChatControlMessageFrame : public ChatControlElement
{
private:
	ChatControlMessage *message;
	int width;
	int height;
	int color;
	Font &dateFont;
	std::wstring wcharDate;
	std::wstring wcharFilename;

	void renderFrame(HDC deviceContext, int x, int y);
	int getHeight(const std::wstring &string);
	int getRemoteResourceHeight();

public:
	ChatControlMessageFrame(ChatControlMessage *message, int width, int color, Font &dateFont);
	virtual ~ChatControlMessageFrame();

	void updateWidth(HWND window, int width);
	void calculateHeight();

	int getHeight();
	ChatControlMessage *getMessage();

	void render(HDC deviceContext, int y, int clientHeight);

};
