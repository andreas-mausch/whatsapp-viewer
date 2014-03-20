#pragma once

#include <windows.h>
#include <vector>

class WhatsappChat;
class WhatsappMessage;

class MainWindow
{
private:
	HWND dialog;
	std::vector<WhatsappChat *> &chats;

	void createChildWindows();
	void setIcon();
	void resizeChildWindows(int width, int height);
	void addChats();
	void addChat(WhatsappChat &chat);
	void addMessage(WhatsappMessage &message);
	void selectChat(int index);
	void selectChat(WhatsappChat &chat);

	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainWindow(std::vector<WhatsappChat *> &chats);
	~MainWindow();

	bool handleMessages();
};
