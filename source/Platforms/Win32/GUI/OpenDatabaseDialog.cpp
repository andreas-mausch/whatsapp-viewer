#include <string>
#include <windows.h>

#include "OpenDatabaseDialog.h"
#include "../StringHelper.h"
#include "../../../../resources/resource.h"

INT_PTR CALLBACK openFileCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	OpenFileDialogStruct *openFileDialogStruct = reinterpret_cast<OpenFileDialogStruct *>(GetWindowLongPtr(dialog, GWLP_USERDATA));

	switch (message)
    {
		case WM_INITDIALOG:
		{
			SetWindowLongPtr(dialog, GWLP_USERDATA, lParam);
		} break;
        case WM_COMMAND:
		{
            switch (LOWORD(wParam))
            {
				case IDC_OPEN_FILE_SELECT:
				{
					WCHAR filename[MAX_PATH];
					memset(filename, 0, sizeof(WCHAR) * MAX_PATH);

					OPENFILENAME openFilename;
					memset(&openFilename, 0, sizeof(OPENFILENAME));
					openFilename.lStructSize = sizeof(OPENFILENAME);
					openFilename.hwndOwner = dialog;
					openFilename.lpstrFile = filename;
					openFilename.nMaxFile = MAX_PATH;
					openFilename.lpstrFilter = L"WhatsApp Databases (*.db; *.crypt5)\0*.db;*.crypt5\0";
					openFilename.lpstrInitialDir = NULL;
					openFilename.lpstrDefExt = L"";
					openFilename.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY;
					openFilename.lpstrTitle = L"Select WhatsApp database file";

					if (GetOpenFileName(&openFilename))
					{
						SetDlgItemText(dialog, IDC_OPEN_FILE_FILENAME, filename);
					}
				} break;
                case IDOK:
				{
					WCHAR filename[MAX_PATH];
					WCHAR accountName[256];

					GetDlgItemText(dialog, IDC_OPEN_FILE_FILENAME, filename, MAX_PATH);
					GetDlgItemText(dialog, IDC_OPEN_FILE_ACCOUNT_NAME, accountName, 256);

					openFileDialogStruct->filename = wstrtostr(filename);
					openFileDialogStruct->accountName = wstrtostr(accountName);

					EndDialog(dialog, LOWORD(wParam));
					return TRUE;
				} break;
                case IDCANCEL:
				{
					EndDialog(dialog, LOWORD(wParam));
					return TRUE;
				} break;
            }
		} break;
    }

	return 0;
}
