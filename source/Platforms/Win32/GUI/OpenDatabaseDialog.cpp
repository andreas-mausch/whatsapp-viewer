#include <string>
#include <sstream>
#include <windows.h>
#include <commctrl.h>

#include "OpenDatabaseDialog.h"
#include "DrawText.h"
#include "../StringHelper.h"
#include "../../../../resources/resource.h"
#include "../Objects/Brush.h"

OpenDatabaseDialog::OpenDatabaseDialog(HWND parent, OpenDatabaseStruct &openDatabaseStruct)
	: openDatabaseStruct(openDatabaseStruct),
	tooltipBrush(CreateSolidBrush(GetSysColor(COLOR_INFOBK))), currentTooltip(NULL),
	Dialog(parent, IDD_OPEN_FILE)
{
}

OpenDatabaseDialog::~OpenDatabaseDialog()
{
}

void OpenDatabaseDialog::selectFile()
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

void OpenDatabaseDialog::selectWadbFile()
{
	WCHAR filename[MAX_PATH];
	memset(filename, 0, sizeof(WCHAR) * MAX_PATH);

	OPENFILENAME openFilename;
	memset(&openFilename, 0, sizeof(OPENFILENAME));
	openFilename.lStructSize = sizeof(OPENFILENAME);
	openFilename.hwndOwner = dialog;
	openFilename.lpstrFile = filename;
	openFilename.nMaxFile = MAX_PATH;
	openFilename.lpstrFilter = L"WhatsApp Databases (*.db)\0*.db\0";
	openFilename.lpstrInitialDir = NULL;
	openFilename.lpstrDefExt = L"";
	openFilename.Flags = OFN_EXPLORER | OFN_ENABLESIZING | OFN_HIDEREADONLY;
	openFilename.lpstrTitle = L"Select WhatsApp database file";

	if (GetOpenFileName(&openFilename))
	{
		SetDlgItemText(dialog, IDC_OPEN_FILE_WADB, filename);
	}
}

void OpenDatabaseDialog::clickOk(WPARAM wParam)
{
	WCHAR filename[MAX_PATH];
	WCHAR accountName[256];
	WCHAR wadbFilename[MAX_PATH];

	GetDlgItemText(dialog, IDC_OPEN_FILE_FILENAME, filename, MAX_PATH);
	GetDlgItemText(dialog, IDC_OPEN_FILE_ACCOUNT_NAME, accountName, 256);
	GetDlgItemText(dialog, IDC_OPEN_FILE_WADB, wadbFilename, MAX_PATH);

	openDatabaseStruct.filename = wstrtostr(filename);
	openDatabaseStruct.accountName = wstrtostr(accountName);
	openDatabaseStruct.wadbFilename = wstrtostr(wadbFilename);

	close(LOWORD(wParam));
}

void OpenDatabaseDialog::hideTooltips()
{
	if (currentTooltip)
	{
		ShowWindow(currentTooltip, SW_HIDE);
		SetWindowPos(dialog, NULL, 0, 0, 600, 210, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		currentTooltip = NULL;
	}
}

void OpenDatabaseDialog::showTooltip(int id)
{
	HWND previousTooltip = currentTooltip;
	HWND tooltipToShow = GetDlgItem(dialog, id);

	hideTooltips();

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

		SetWindowPos(dialog, NULL, 0, 0, 600, 210 + height, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		SetWindowPos(currentTooltip, NULL, 10, 120, clientRect.right - 20, height, SWP_NOACTIVATE | SWP_NOOWNERZORDER | SWP_NOZORDER | SWP_SHOWWINDOW);
	}
}

INT_PTR OpenDatabaseDialog::callback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			SetDlgItemText(dialog, IDC_OPEN_FILE_FILENAME, strtowstr(openDatabaseStruct.filename).c_str());
			SetDlgItemText(dialog, IDC_OPEN_FILE_ACCOUNT_NAME, strtowstr(openDatabaseStruct.accountName).c_str());
			SetDlgItemText(dialog, IDC_OPEN_FILE_WADB, strtowstr(openDatabaseStruct.wadbFilename).c_str());

			SetWindowPos(dialog, NULL, 0, 0, 600, 210, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		} break;
		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = reinterpret_cast<HDC>(wParam);
			HWND window = reinterpret_cast<HWND>(lParam);

			if (window == GetDlgItem(dialog, IDC_OPEN_FILE_FILENAME_TOOLTIP)
				|| window == GetDlgItem(dialog, IDC_OPEN_FILE_ACCOUNT_NAME_TOOLTIP)
				|| window == GetDlgItem(dialog, IDC_OPEN_FILE_WADB_TOOLTIP))
			{
				SetTextColor(hdcStatic, GetSysColor(COLOR_INFOTEXT));
				SetBkColor(hdcStatic, GetSysColor(COLOR_INFOBK));

				return reinterpret_cast<INT_PTR>(tooltipBrush.get());
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
							selectFile();
						} break;
						case IDC_OPEN_FILE_WADB_SELECT:
						{
							selectWadbFile();
						} break;
						case IDC_OPEN_FILE_FILENAME_TOOLTIP_BUTTON:
						{
							showTooltip(IDC_OPEN_FILE_FILENAME_TOOLTIP);
						} break;
						case IDC_OPEN_FILE_ACCOUNT_NAME_TOOLTIP_BUTTON:
						{
							showTooltip(IDC_OPEN_FILE_ACCOUNT_NAME_TOOLTIP);
						} break;
						case IDC_OPEN_FILE_WADB_TOOLTIP_BUTTON:
						{
							showTooltip(IDC_OPEN_FILE_WADB_TOOLTIP);
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
		case WM_SIZE:
		{
			RECT clientRect;
			GetClientRect(dialog, &clientRect);

			SetWindowPos(GetDlgItem(dialog, IDOK), NULL, 420, clientRect.bottom - 40, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
			SetWindowPos(GetDlgItem(dialog, IDCANCEL), NULL, 505, clientRect.bottom - 40, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOOWNERZORDER | SWP_NOZORDER);
		} break;
	}

	return 0;
}
