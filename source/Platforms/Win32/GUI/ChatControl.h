#pragma once

#include <vector>

class ChatControlMessageFrame;
class Font;
class ImageDecoder;
class Smileys;
class WhatsappChat;

#define WM_CHATCONTROL (WM_USER+0)

enum ChatControlMessages
{
	CHAT_CONTROL_SETCHAT,
	CHAT_CONTROL_START_RESIZING_MESSAGES,
	CHAT_CONTROL_STOP_RESIZING_MESSAGES,
	CHAT_CONTROL_REDRAW
};

class ChatControl
{
private:
	ImageDecoder *imageDecoder;
	Smileys *smileys;

	HWND window;
	Font *dateFont;
	HDC backbuffer;
	HBITMAP backbufferBitmap;
	WhatsappChat *chat;
	std::vector<ChatControlMessageFrame *> messages;
	bool shouldResizeMessages;

	static LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void setChat(WhatsappChat &chat);

	void buildMessages();
	void clearMessages();
	void calculateScrollInfo();
	void resizeMessages();
	void resizeMessageWidths();

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
