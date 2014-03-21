#pragma once

#include <vector>

class WhatsappChat;
class ChatControlMessage;

#define WM_CHATCONTROL_SETCHAT (WM_USER+0)

class ChatControl
{
private:
	HWND window;
	HGDIOBJ dateFont;
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

	LRESULT onScroll(WPARAM wParam);

public:

	ChatControl(HWND window);
	~ChatControl();

	static void registerChatControl();

};
