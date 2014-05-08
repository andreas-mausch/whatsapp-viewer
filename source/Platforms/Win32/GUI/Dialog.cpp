#include <windows.h>

#include "Dialog.h"
#include "../../../Exceptions/Exception.h"

Dialog::Dialog(HWND parent, int resourceId)
	: parent(parent), resourceId(resourceId)
{
}

Dialog::~Dialog()
{
	close();
}

void Dialog::open()
{
	CreateDialogParam(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceId), parent, dialogCallback, reinterpret_cast<LPARAM>(this));

	if (!dialog)
	{
		throw Exception("could not create dialog");
	}
}

int Dialog::openModal()
{
	return DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(resourceId), parent, dialogCallback, reinterpret_cast<LPARAM>(this));
}

void Dialog::show()
{
	ShowWindow(dialog, SW_SHOW);
}

void Dialog::hide()
{
	ShowWindow(dialog, SW_HIDE);
}

void Dialog::close()
{
	close(0);
}

void Dialog::close(int code)
{
	if (dialog != NULL)
	{
		SetWindowLongPtr(dialog, DWLP_USER, 0);
		EndDialog(dialog, code);
	}

	dialog = NULL;
}

HWND Dialog::getHandle()
{
	return dialog;
}

INT_PTR CALLBACK Dialog::dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			Dialog *dialogObject = reinterpret_cast<Dialog *>(lParam);
			dialogObject->dialog = dialog;

			SetWindowLongPtr(dialog, DWLP_USER, lParam);
		} break;
	}

	Dialog *dialogObject = reinterpret_cast<Dialog *>(GetWindowLongPtr(dialog, DWLP_USER));

	if (dialogObject != NULL)
	{
		return dialogObject->callback(dialog, message, wParam, lParam);
	}

	return 0;
}
