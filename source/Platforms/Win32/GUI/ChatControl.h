#pragma once

#include <vector>

class WhatsappChat;
class ChatControlMessage;
class JpegDecoder;

#define WM_CHATCONTROL_SETCHAT (WM_USER+0)

class ChatControl
{
private:
	JpegDecoder *jpegDecoder;

	HWND window;
	HGDIOBJ dateFont;
	HDC backbuffer;
	HBITMAP backbufferBitmap;
	WhatsappChat *chat;
	std::vector<ChatControlMessage *> messages;

	static LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void buildMessages();
	void clearMessages();
	void calculateMessageHeights();
	void calculateScrollInfo();

	LRESULT onPaint();
	void drawMessage(ChatControlMessage &message, HDC deviceContext, int y, int clientRectWidth, HGDIOBJ dateFont);
	void redraw();

	void scroll(int newPosition);
	LRESULT onScroll(WPARAM wParam);
	LRESULT onMousewheel(int delta);

	void createBackbuffer();
	void deleteBackbuffer();

public:

	ChatControl(HWND window);
	~ChatControl();

	static void registerChatControl();

};
