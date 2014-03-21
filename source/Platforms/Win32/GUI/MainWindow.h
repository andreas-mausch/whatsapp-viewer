#pragma once

#include <windows.h>
#include <vector>

class WhatsappChat;
class WhatsappMessage;
class WhatsappDatabase;

class MainWindow
{
private:
	HWND dialog;
	std::vector<WhatsappChat *> chats;
	WhatsappDatabase *database;

	void openDatabase();
	void clearChats();

	void createChildWindows();
	void setIcon();
	void resizeChildWindows(int width, int height);
	void addChats();
	void addChat(WhatsappChat &chat);
	void selectChat(int index);
	void selectChat(WhatsappChat &chat);

	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainWindow();
	~MainWindow();

	bool handleMessages();
};
