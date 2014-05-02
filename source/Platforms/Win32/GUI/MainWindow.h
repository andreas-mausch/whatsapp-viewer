#pragma once

#include <windows.h>
#include <vector>

#include "ListViewSorting.h"
#include "OpenDatabaseDialog.h"
#include "../Objects/Accelerator.h"

class AboutDialog;
class Exception;
class Settings;
class WhatsappChat;
class WhatsappMessage;
class WhatsappDatabase;

enum SortingDirection
{
	SORTING_DIRECTION_ASCENDING,
	SORTING_DIRECTION_DESCENDING
};

class MainWindow
{
private:
	HWND dialog;
	AboutDialog *aboutDialog;
	Accelerator accelerator;
	std::vector<WhatsappChat *> chats;
	WhatsappDatabase *database;
	std::string tempFilename;
	OpenDatabaseStruct lastDatabaseOpened;
	Settings &settings;
	int sortingColumn;
	SortingDirection sortingDirection;

	void openDatabase();
	void openPlainDatabase(const std::string &filename);
	void closeDatabase();

	void decryptDatabase();
	void clearChats();
	void clearChatList();

	void createChildWindows();
	void setIcon();
	void resizeChildWindows(int width, int height);
	void addChats();
	void addChat(WhatsappChat &chat);
	void selectChat(WhatsappChat *chat);
	void sortChats();
	void setSortingColumn(int columnIndex);
	void updateSortingArrow();

	void exportChat(WhatsappChat &chat);

	void getTempFilename();
	bool fileExists(const std::string &filename);

	void readSettings();

	void showAboutDialog();
	void close();

	void onDrop(HDROP drop);

	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);
	INT_PTR handleMessage(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

	static int CALLBACK sortingCallback(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

public:
	MainWindow(Settings &settings);
	~MainWindow();

	bool handleMessages();
	void openDatabase(const std::string &filename);

	static void displayException(HWND mainWindow, Exception &exception);
};
