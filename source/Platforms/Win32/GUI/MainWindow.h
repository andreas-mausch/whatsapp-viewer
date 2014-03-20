#pragma once

#include <windows.h>

class MainWindow
{
private:
	HWND dialog;

	void createChildWindows();
	void resizeChildWindows(int width, int height);
	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

public:
	MainWindow();
	~MainWindow();

	void handleMessages();
};
