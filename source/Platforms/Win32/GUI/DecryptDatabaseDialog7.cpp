#include <windows.h>

#include "DecryptDatabaseDialog7.h"
#include "../StringHelper.h"
#include "../../../../resources/resource.h"

DecryptDatabaseDialog7::DecryptDatabaseDialog7(HWND parent, OpenDatabaseStruct &openDatabaseStruct)
	: openDatabaseStruct(openDatabaseStruct), Dialog(parent, IDD_DECRYPT_CRYPT7)
{
}

DecryptDatabaseDialog7::~DecryptDatabaseDialog7()
{
}

void DecryptDatabaseDialog7::selectDatabaseFile()
{
	WCHAR filename[MAX_PATH];
	memset(filename, 0, sizeof(WCHAR) * MAX_PATH);

	OPENFILENAME openFilename;
	memset(&openFilename, 0, sizeof(OPENFILENAME));
	openFilename.lStructSize = sizeof(OPENFILENAME);
	openFilename.hwndOwner = dialog;
	openFilename.lpstrFile = filename;
	openFilename.nMaxFile = MAX_PATH;
	openFilename.lpstrFilter = L"WhatsApp Databases (*.crypt7, *.crypt8)\0*.crypt7;*.crypt8\0";
	openFilename.lpstrInitialDir = NULL;
	openFilename.lpstrDefExt = L"";
	openFilename.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY;
	openFilename.lpstrTitle = L"Select WhatsApp database file";

	if (GetOpenFileName(&openFilename))
	{
		SetDlgItemText(dialog, IDC_DECRYPT_CRYPT7_DATABASE, filename);
	}
}

void DecryptDatabaseDialog7::selectKeyFile()
{
	WCHAR filename[MAX_PATH];
	memset(filename, 0, sizeof(WCHAR) * MAX_PATH);

	OPENFILENAME openFilename;
	memset(&openFilename, 0, sizeof(OPENFILENAME));
	openFilename.lStructSize = sizeof(OPENFILENAME);
	openFilename.hwndOwner = dialog;
	openFilename.lpstrFile = filename;
	openFilename.nMaxFile = MAX_PATH;
	openFilename.lpstrFilter = L"WhatsApp Key (key)\0key\0";
	openFilename.lpstrInitialDir = NULL;
	openFilename.lpstrDefExt = L"";
	openFilename.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY;
	openFilename.lpstrTitle = L"Select WhatsApp key file";

	if (GetOpenFileName(&openFilename))
	{
		SetDlgItemText(dialog, IDC_DECRYPT_CRYPT7_KEYFILE, filename);
	}
}

void DecryptDatabaseDialog7::clickOk(WPARAM wParam)
{
	WCHAR filename[MAX_PATH];
	WCHAR keyFilename[256];

	GetDlgItemText(dialog, IDC_DECRYPT_CRYPT7_DATABASE, filename, MAX_PATH);
	GetDlgItemText(dialog, IDC_DECRYPT_CRYPT7_KEYFILE, keyFilename, 256);

	openDatabaseStruct.filename = wstrtostr(filename);
	openDatabaseStruct.keyFilename = wstrtostr(keyFilename);

	close(LOWORD(wParam));
}

INT_PTR DecryptDatabaseDialog7::callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			SetDlgItemText(dialog, IDC_DECRYPT_CRYPT7_DATABASE, strtowstr(openDatabaseStruct.filename).c_str());
			SetDlgItemText(dialog, IDC_DECRYPT_CRYPT7_KEYFILE, strtowstr(openDatabaseStruct.keyFilename).c_str());
		} break;
		case WM_COMMAND:
		{
			switch (HIWORD(wParam))
			{
				case BN_CLICKED:
				{
					switch (LOWORD(wParam))
					{
						case IDC_DECRYPT_CRYPT7_SELECT_DATABASE:
						{
							selectDatabaseFile();
						} break;
						case IDC_DECRYPT_CRYPT7_SELECT_KEY:
						{
							selectKeyFile();
						} break;
						case IDOK:
						{
							clickOk(wParam);
							return TRUE;
						} break;
						case IDCANCEL:
						{
							close(LOWORD(wParam));
							return TRUE;
						} break;
					}
				} break;
			}
		} break;
	}

	return 0;
}
