#include <windows.h>
#include <commctrl.h>

#include "MainWindow.h"
#include "../../../../resources/resource.h"
#include "../../../Exceptions/Exception.h"
#include "../../../WhatsApp/Chat.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

std::wstring strtowstr(const std::string &text)
{
	std::wstring wText;
    WCHAR *szWText = new WCHAR[text.length() + 1];
    szWText[text.size()] = '\0';
    MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, szWText, static_cast<int>(text.length()));
    wText = szWText;
    delete[] szWText;
    return wText;
}

MainWindow::MainWindow(std::vector<WhatsappChat *> &chats) : chats(chats)
{
	CoInitialize(NULL);

	INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS;
    InitCommonControlsEx(&icex); 

	CreateDialogParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_MAIN),
		NULL,
		dialogCallback,
		reinterpret_cast<LPARAM>(this));

	addChats();

	ShowWindow(dialog, SW_SHOW);
}

MainWindow::~MainWindow()
{
}

bool MainWindow::handleMessages()
{
	MSG message;
	BOOL ret = GetMessage(&message, 0, 0, 0);

	if (ret == -1)
	{
		return false;
	}
	else if (ret == 0)
	{
		// WM_QUIT
		return false;
	}
	else
	{
		if (!IsDialogMessage(dialog, &message))
		{
			TranslateMessage(&message);
			DispatchMessage(&message);
		}
	}

	return true;
}

void MainWindow::createChildWindows()
{
	// create the status bar at the bottom of the window
	CreateWindowEx(0, STATUSCLASSNAME, L"file manager", WS_CHILD, 0, 0, 0, 0, dialog, reinterpret_cast<HMENU>(IDC_MAIN_STATUS), GetModuleHandle(NULL), 0);

	// create list view columns
	WCHAR columnsStrings[][256] = { L"phone number", L"last message" };
	DWORD columnsWidths[] = { 250, 80 };

	for (DWORD i = 0; i < 2; i++)
	{
		LVCOLUMN column;
		ZeroMemory(&column, sizeof(LVCOLUMN));

		column.mask = LVCF_TEXT | LVCF_WIDTH;
		column.cx = columnsWidths[i];
		column.pszText = columnsStrings[i];

		ListView_InsertColumn(GetDlgItem(dialog, IDC_MAIN_CHATS), i, &column);
	}

	// set the image list for the list view, tree view and combo box
	// ListView_SetImageList(GetDlgItem(dialog, IDC_MAIN_CHATS), windowFilemanager->m_imageList, LVSIL_SMALL);

	addChats();
}

void MainWindow::addChats()
{
	for (std::vector<WhatsappChat *>::iterator it = chats.begin(); it != chats.end(); ++it)
	{
		addChat(**it);
	}
}

void MainWindow::addChat(WhatsappChat &chat)
{
	WCHAR text[256];
	wcscpy_s(text, strtowstr(chat.getKey()).c_str());

	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));

	item.iItem = ListView_GetItemCount(GetDlgItem(dialog, IDC_MAIN_CHATS));
	item.mask = LVIF_TEXT;
	item.pszText = text;
	// item.iImage = GetFileIconIndex(fileEntry);
	ListView_InsertItem(GetDlgItem(dialog, IDC_MAIN_CHATS), &item);
}

void MainWindow::resizeChildWindows(int width, int height)
{
}

INT_PTR MainWindow::dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	MainWindow *mainWindow = reinterpret_cast<MainWindow *>(GetWindowLongPtr(dialog, 0));

	switch (message)
	{
		case WM_INITDIALOG:
		{
			mainWindow = reinterpret_cast<MainWindow *>(lParam);

			if (!mainWindow)
			{
				throw Exception("could not create main window: invalid pointer");
			}

			// save the pointer to the class
			SetWindowLongPtr(dialog, 0, reinterpret_cast<LONG_PTR>(mainWindow));

			// save the dialog handle of the dialog box
			mainWindow->dialog = dialog;

			mainWindow->createChildWindows();
		} break;
		case WM_COMMAND:
		{
			switch(HIWORD(wParam))
			{
				case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
						case ID_MENU_MAIN_FILE_OPEN:
						{
						} break;
					}
				} break;
			}
		} break;
		case WM_NOTIFY:
		{
			NMHDR *hdr																		= reinterpret_cast<NMHDR *>(lParam);

			if (!hdr)
			{
				break;
			}

			switch (hdr->code)
			{
				case TVN_SELCHANGED:
				{
					switch (hdr->idFrom)
					{
						case IDC_MAIN_CHATS:
						{
						} break;
					}
				} break;
				case NM_DBLCLK:
				{
					switch (hdr->idFrom)
					{
						case IDC_MAIN_CHATS:
						{
						} break;
					}
				} break;
			}
		} break;
		case WM_SIZE:
		{
			if (mainWindow)
			{
				mainWindow->resizeChildWindows(LOWORD(lParam), HIWORD(lParam));
			}
		} break;
		case WM_CLOSE:
		{
			DestroyWindow(dialog);
			return TRUE;
		} break;
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return TRUE;
		} break;
	}

	return 0;
}
