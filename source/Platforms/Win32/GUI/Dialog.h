#pragma once

#define WM_DIALOG (WM_USER+1)

enum DialogMessage
{
	DIALOG_CLOSED
};

class Dialog
{
private:
	static INT_PTR CALLBACK dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam);

protected:
	HWND parent;
	HWND dialog;
	int resourceId;

	virtual INT_PTR callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam) = 0;

public:
	Dialog(HWND parent, int resourceId);
	virtual ~Dialog();

	void open();
	int openModal();

	void show();
	void hide();

	void close();
	void close(int code);

	HWND getHandle();

};
