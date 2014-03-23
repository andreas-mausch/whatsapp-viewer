#pragma once

#include <windows.h>
#include <vector>

class Exception;
class WhatsappChat;
class WhatsappMessage;
class WhatsappDatabase;

class MainWindow
{
private:
	HWND dialog;
	std::vector<WhatsappChat *> chats;
	WhatsappDatabase *database;
	std::string tempFilename;

	void openDatabase();
	void clearChats();

	void createChildWindows();
	void setIcon();
	void resizeChildWindows(int width, int height);
	void addChats();
	void addChat(WhatsappChat &chat);
	void selectChat(int index);
	void selectChat(WhatsappChat &chat);

	void getTempFilename();
	bool fileExists(const std::string &filename);

	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainWindow();
	~MainWindow();

	bool handleMessages();

	void displayException(Exception &exception);
};
