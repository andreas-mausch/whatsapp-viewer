#pragma once

#include <vector>
#include "../../CriticalSectionLock.h"

class AnimatedGif;
class BuildMessagesThread;
class ChatControlMessageFrame;
class Font;
class ImageDecoder;
class LoadingAnimationThread;
class ResizeMessagesThread;
class Smileys;
class Thread;
class WhatsappChat;

#define WM_CHATCONTROL (WM_USER+0)

enum ChatControlMessages
{
	CHAT_CONTROL_SETCHAT,
	CHAT_CONTROL_START_RESIZING_MESSAGES,
	CHAT_CONTROL_STOP_RESIZING_MESSAGES,
	CHAT_CONTROL_REDRAW,
	CHAT_CONTROL_BUILDING_MESSAGES_FINISHED,
	CHAT_CONTROL_RESIZING_MESSAGES_FINISHED
};

class ChatControl
{
private:
	CriticalSectionLock lock;
	ImageDecoder *imageDecoder;
	Smileys *smileys;
	AnimatedGif *loadingAnimation;

	HWND window;
	Font *dateFont;
	HDC backbuffer;
	HBITMAP backbufferBitmap;
	WhatsappChat *chat;
	std::vector<ChatControlMessageFrame *> messages;
	bool shouldResizeMessages;

	static LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

	void setChat(WhatsappChat *chat);

	void calculateScrollInfo();

	LRESULT onPaint();
	void drawMessage(ChatControlMessageFrame &messageFrame, HDC deviceContext, int y, int clientRectWidth, HGDIOBJ dateFont);
	void redraw();

	void scroll(int newPosition);
	LRESULT onScroll(WPARAM wParam);
	LRESULT onMousewheel(int delta);

	void createBackbuffer();
	void paintBackbuffer();
	void deleteBackbuffer();

	void startBuildingMessages();
	void stopBuildingMessages();
	BuildMessagesThread *buildMessagesThread;

	void startResizingMessages();
	void stopResizingMessages();
	ResizeMessagesThread *resizeMessagesThread;

	volatile bool painting;
	int totalMessagesHeight;

	void startLoadingAnimation();
	void stopLoadingAnimation();
	LoadingAnimationThread *loadingAnimationThread;

	void interruptJoinAndDeleteThread(Thread &thread);

public:

	ChatControl(HWND window);
	~ChatControl();

	static void registerChatControl();

};
