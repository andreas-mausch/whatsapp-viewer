#pragma once

#include <vector>

class WhatsappChat;
class ChatControlMessageFrame;
class ImageDecoder;
class Smileys;

#define WM_CHATCONTROL_SETCHAT (WM_USER+0)
#define WM_CHATCONTROL_REPAINT (WM_USER+1)

class ChatControl
{
private:
	ImageDecoder *imageDecoder;
	Smileys *smileys;

	HWND window;
	HFONT dateFont;
	HDC backbuffer;
	HBITMAP backbufferBitmap;
	WhatsappChat *chat;
	std::vector<ChatControlMessageFrame *> messages;

	static LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void setChat(WhatsappChat &chat);

	void buildMessages();
	void clearMessages();
	void calculateScrollInfo();
	void resizeMessages();

	LRESULT onPaint();
	void drawMessage(ChatControlMessageFrame &messageFrame, HDC deviceContext, int y, int clientRectWidth, HGDIOBJ dateFont);
	void redraw();

	void scroll(int newPosition);
	LRESULT onScroll(WPARAM wParam);
	LRESULT onMousewheel(int delta);

	void createBackbuffer();
	void paintBackbuffer();
	void deleteBackbuffer();

public:

	ChatControl(HWND window);
	~ChatControl();

	static void registerChatControl();

};
