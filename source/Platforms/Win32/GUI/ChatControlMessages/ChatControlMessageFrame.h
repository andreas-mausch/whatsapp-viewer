#pragma once

class ChatControlMessage;

class ChatControlMessageFrame
{
private:
	ChatControlMessage *message;
	int width;
	int height;
	int color;
	HFONT dateFont;
	std::wstring wcharDate;

	void renderFrame(HDC deviceContext, int x, int y);
	int getDateHeight();
	int getRemoteResourceHeight();

public:
	ChatControlMessageFrame(ChatControlMessage *message, int width, int color, HFONT dateFont);
	virtual ~ChatControlMessageFrame();

	void updateWidth(HWND window, int width);
	void calculateHeight();

	int getHeight();
	ChatControlMessage *getMessage();

	void render(HDC deviceContext, int x, int y, int clientHeight);

};
