#include <string>
#include <sstream>
#include <windows.h>
#include <commctrl.h>

#include "OpenDatabaseDialog.h"
#include "DrawText.h"
#include "../StringHelper.h"
#include "../../../../resources/resource.h"
#include "../Objects/Brush.h"

void selectFile(HWND dialog)
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
}

void ok(HWND dialog, OpenDatabaseStruct *openDatabaseStruct, WPARAM wParam)
{
	WCHAR filename[MAX_PATH];
	WCHAR accountName[256];

	GetDlgItemText(dialog, IDC_OPEN_FILE_FILENAME, filename, MAX_PATH);
	GetDlgItemText(dialog, IDC_OPEN_FILE_ACCOUNT_NAME, accountName, 256);

	openDatabaseStruct->filename = wstrtostr(filename);
	openDatabaseStruct->accountName = wstrtostr(accountName);

	EndDialog(dialog, LOWORD(wParam));
}

Brush brush(CreateSolidBrush(GetSysColor(COLOR_INFOBK)));
HWND currentTooltip = NULL;

void hideTooltips(HWND dialog)
{
	if (currentTooltip)
	{
		ShowWindow(currentTooltip, SW_HIDE);
		SetWindowPos(dialog, NULL, 0, 0, 500, 180, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		currentTooltip = NULL;
	}
}

void showTooltip(HWND dialog, int id)
{
	HWND previousTooltip = currentTooltip;
	HWND tooltipToShow = GetDlgItem(dialog, id);

	hideTooltips(dialog);

	if (previousTooltip != tooltipToShow)
	{
		currentTooltip = tooltipToShow;

		RECT clientRect;
		GetClientRect(dialog, &clientRect);

		WCHAR text[256];
		GetDlgItemText(dialog, id, text, 256);

		HDC deviceContext = GetDC(dialog);
		int height = calculateDrawTextHeight(deviceContext, text, clientRect.right - 20, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));
		ReleaseDC(dialog, deviceContext);

		SetWindowPos(dialog, NULL, 0, 0, 500, 180 + height, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(currentTooltip, NULL, 10, 100, clientRect.right - 20, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
}

INT_PTR CALLBACK openDatabaseDialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	OpenDatabaseStruct *openDatabaseStruct = reinterpret_cast<OpenDatabaseStruct *>(GetWindowLongPtr(dialog, GWLP_USERDATA));

	switch (message)
    {
		case WM_INITDIALOG:
		{
			openDatabaseStruct = reinterpret_cast<OpenDatabaseStruct *>(lParam);
			SetWindowLongPtr(dialog, GWLP_USERDATA, lParam);

			SetDlgItemText(dialog, IDC_OPEN_FILE_FILENAME, strtowstr(openDatabaseStruct->filename).c_str());
			SetDlgItemText(dialog, IDC_OPEN_FILE_ACCOUNT_NAME, strtowstr(openDatabaseStruct->accountName).c_str());

			SetWindowPos(dialog, NULL, 0, 0, 500, 180, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		} break;
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = reinterpret_cast<HDC>(wParam);
			HWND window = reinterpret_cast<HWND>(lParam);

			if (window == GetDlgItem(dialog, IDC_OPEN_FILE_FILENAME_TOOLTIP)
				|| window == GetDlgItem(dialog, IDC_OPEN_FILE_ACCOUNT_NAME_TOOLTIP))
			{
				SetTextColor(hdcStatic, GetSysColor(COLOR_INFOTEXT));
				SetBkColor(hdcStatic, GetSysColor(COLOR_INFOBK));

				return reinterpret_cast<INT_PTR>(brush.get());
			}
		} break;
        case WM_COMMAND:
		{
			switch (HIWORD(wParam))
			{
				case BN_CLICKED:
				{
					switch (LOWORD(wParam))
					{
						case IDC_OPEN_FILE_SELECT:
						{
							selectFile(dialog);
						} break;
						case IDC_OPEN_FILE_FILENAME_TOOLTIP_BUTTON:
						{
							showTooltip(dialog, IDC_OPEN_FILE_FILENAME_TOOLTIP);
						} break;
						case IDC_OPEN_FILE_ACCOUNT_NAME_TOOLTIP_BUTTON:
						{
							showTooltip(dialog, IDC_OPEN_FILE_ACCOUNT_NAME_TOOLTIP);
						} break;
						case IDOK:
						{
							ok(dialog, openDatabaseStruct, wParam);
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
		} break;
		case WM_SIZE:
		{
			RECT clientRect;
			GetClientRect(dialog, &clientRect);

			SetWindowPos(GetDlgItem(dialog, IDOK), NULL, 320, clientRect.bottom - 40, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(GetDlgItem(dialog, IDCANCEL), NULL, 405, clientRect.bottom - 40, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		} break;
    }

	return 0;
}

INT_PTR CALLBACK decryptDatabaseDialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
    {
		case WM_INITDIALOG:
		{
			SetWindowText(dialog, L"Decrypt WhatsApp Database");
		} break;
    }

	return openDatabaseDialogCallback(dialog, message, wParam, lParam);
}
