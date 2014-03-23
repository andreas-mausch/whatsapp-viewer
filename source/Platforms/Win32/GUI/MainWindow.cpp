#include <windows.h>
#include <commctrl.h>
#include <vector>

#include "ChatControl.h"
#include "MainWindow.h"
#include "OpenDatabaseDialog.h"
#include "../../../../resources/resource.h"
#include "../../../Exceptions/Exception.h"
#include "../../../WhatsApp/Chat.h"
#include "../../../WhatsApp/Crypt5.h"
#include "../../../WhatsApp/Database.h"
#include "../../../WhatsApp/Message.h"
#include "../StringHelper.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

MainWindow::MainWindow() : database(NULL)
{
	CoInitialize(NULL);

	INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

	ChatControl::registerChatControl();

	CreateDialogParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_MAIN),
		NULL,
		dialogCallback,
		reinterpret_cast<LPARAM>(this));

	ShowWindow(dialog, SW_SHOW);
}

MainWindow::~MainWindow()
{
	delete database;
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
	setIcon();

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

	ListView_SetExtendedListViewStyleEx(GetDlgItem(dialog, IDC_MAIN_CHATS), 0, LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	// set the image list for the list view, tree view and combo box
	// ListView_SetImageList(GetDlgItem(dialog, IDC_MAIN_CHATS), windowFilemanager->m_imageList, LVSIL_SMALL);
}

void MainWindow::setIcon()
{
	HICON icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON));
	SendMessage(dialog, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(icon));
	SendMessage(dialog, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(icon));
}

void MainWindow::addChats()
{
	ListView_DeleteAllItems(GetDlgItem(dialog, IDC_MAIN_CHATS));

	for (std::vector<WhatsappChat *>::iterator it = chats.begin(); it != chats.end(); ++it)
	{
		addChat(**it);
	}
}

void MainWindow::addChat(WhatsappChat &chat)
{
	WCHAR *text = buildWcharString(chat.getKey());

	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));

	item.iItem = ListView_GetItemCount(GetDlgItem(dialog, IDC_MAIN_CHATS));
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText = text;
	item.lParam = reinterpret_cast<LPARAM>(&chat);
	// item.iImage = GetFileIconIndex(fileEntry);
	ListView_InsertItem(GetDlgItem(dialog, IDC_MAIN_CHATS), &item);

	delete[] text;
}

void MainWindow::selectChat(WhatsappChat &chat)
{
	SendDlgItemMessage(dialog, IDC_MAIN_MESSAGES, WM_CHATCONTROL_SETCHAT, 0, reinterpret_cast<LPARAM>(&chat));
}

void MainWindow::resizeChildWindows(int width, int height)
{
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_CHATS), NULL, 15, 15, 400, height - 30, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_MESSAGES), NULL, 430, 15, width - 445, height - 30, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void MainWindow::clearChats()
{
	for (std::vector<WhatsappChat *>::iterator it = chats.begin(); it != chats.end(); ++it)
	{
		delete *it;
	}
	chats.clear();
}

void MainWindow::openDatabase()
{
	OpenFileDialogStruct openFileDialogStruct;
	if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPEN_FILE), dialog, openFileCallback, reinterpret_cast<LPARAM>(&openFileDialogStruct)) == IDOK)
	{
		clearChats();
		delete database;

		try
		{
			unsigned char key[24];
			buildKey(key, openFileDialogStruct.accountName);

			decryptWhatsappDatabase(openFileDialogStruct.filename, "msgstore.db", key);

			database = new WhatsappDatabase("msgstore.db");
			database->getChats(chats);

			addChats();
		}
		catch (Exception &exception)
		{
			displayException(exception);
		}
	}
}

void MainWindow::displayException(Exception &exception)
{
	WCHAR *cause = buildWcharString(exception.getCause());
	MessageBox(dialog, cause, L"Error", MB_OK | MB_ICONERROR);
	delete[] cause;
}

INT_PTR MainWindow::dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	MainWindow *mainWindow = reinterpret_cast<MainWindow *>(GetWindowLongPtr(dialog, GWLP_USERDATA));

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
			SetWindowLongPtr(dialog, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(mainWindow));

			// save the dialog handle of the dialog box
			mainWindow->dialog = dialog;

			mainWindow->createChildWindows();
		} break;
		case WM_COMMAND:
		{
			switch(LOWORD(wParam))
			{
				case ID_MENU_MAIN_FILE_OPEN:
				{
					mainWindow->openDatabase();
				} break;
			}
		} break;
		case WM_NOTIFY:
		{
			NMHDR *hdr = reinterpret_cast<NMHDR *>(lParam);

			if (!hdr)
			{
				break;
			}

			switch (hdr->code)
			{
				case LVN_ITEMCHANGED:
				{
					switch (hdr->idFrom)
					{
						case IDC_MAIN_CHATS:
						{
							NMLISTVIEW *nmListView = reinterpret_cast<NMLISTVIEW *>(lParam);

							if (nmListView->uChanged & LVIF_STATE &&
								nmListView->uNewState & LVIS_SELECTED)
							{
								LVITEM item;
								ZeroMemory(&item, sizeof(LVITEM));
								item.mask = LVIF_PARAM;
								item.iItem = nmListView->iItem;
								ListView_GetItem(GetDlgItem(dialog, IDC_MAIN_CHATS), &item);

								WhatsappChat &chat = *reinterpret_cast<WhatsappChat *>(item.lParam);
								mainWindow->selectChat(chat);
							}
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
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO *minmaxinfo = reinterpret_cast<MINMAXINFO *>(lParam);
			minmaxinfo->ptMinTrackSize.x = 500;
			minmaxinfo->ptMinTrackSize.y = 200;
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
