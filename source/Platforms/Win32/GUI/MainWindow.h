#pragma once

#include <windows.h>
#include <vector>

#include "OpenDatabaseDialog.h"

class Exception;
class Settings;
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
	OpenDatabaseStruct lastDatabaseOpened;
	Settings &settings;

	void openDatabase();
	void decryptDatabase();
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

	void readSettings();

	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainWindow(Settings &settings);
	~MainWindow();

	bool handleMessages();

	void displayException(Exception &exception);
};
