#include <windows.h>

#include "DecryptDatabaseDialog5.h"

DecryptDatabaseDialog5::DecryptDatabaseDialog5(HWND parent, OpenDatabaseStruct &openDatabaseStruct)
	: OpenDatabaseDialog(parent, openDatabaseStruct)
{
}

DecryptDatabaseDialog5::~DecryptDatabaseDialog5()
{
}

INT_PTR DecryptDatabaseDialog5::callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			SetWindowText(dialog, L"Decrypt WhatsApp Database");
		} break;
	}

	return OpenDatabaseDialog::callback(dialog, message, wParam, lParam);
}
