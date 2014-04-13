#include <windows.h>
#include <commctrl.h>
#include <fstream>
#include <vector>

#include "ChatControl.h"
#include "MainWindow.h"
#include "OpenDatabaseDialog.h"
#include "../../../ChatExporter.h"
#include "../../../Settings.h"
#include "../../../../resources/resource.h"
#include "../../../Exceptions/Exception.h"
#include "../../../WhatsApp/Chat.h"
#include "../../../WhatsApp/Crypt5.h"
#include "../../../WhatsApp/Database.h"
#include "../../../WhatsApp/Message.h"
#include "../../../VectorUtils.h"
#include "../StringHelper.h"

#pragma comment(linker, \
  "\"/manifestdependency:type='Win32' "\
  "name='Microsoft.Windows.Common-Controls' "\
  "version='6.0.0.0' "\
  "processorArchitecture='*' "\
  "publicKeyToken='6595b64144ccf1df' "\
  "language='*'\"")

MainWindow::MainWindow(Settings &settings)
	: settings(settings), database(NULL)
{
	CoInitialize(NULL);

	INITCOMMONCONTROLSEX icex;
    icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC  = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES;
    InitCommonControlsEx(&icex);

	ChatControl::registerChatControl();

	getTempFilename();

	readSettings();

	CreateDialogParam(GetModuleHandle(NULL),
		MAKEINTRESOURCE(IDD_MAIN),
		NULL,
		dialogCallback,
		reinterpret_cast<LPARAM>(this));

	ShowWindow(dialog, SW_SHOW);
}

MainWindow::~MainWindow()
{
	closeDatabase();

	if (fileExists(tempFilename))
	{
		WCHAR *filenameWchar = buildWcharString(tempFilename);
		DeleteFile(filenameWchar);
		delete[] filenameWchar;
	}
}

void MainWindow::readSettings()
{
	try
	{
		lastDatabaseOpened.filename = settings.read("lastOpenedFile");
		lastDatabaseOpened.accountName = settings.read("lastOpenedAccount");
	}
	catch (Exception &exception)
	{
	}
}

bool MainWindow::fileExists(const std::string &filename)
{
	WCHAR *filenameWchar = buildWcharString(filename);
	DWORD attributes = GetFileAttributes(filenameWchar);
	delete[] filenameWchar;

	return (attributes != INVALID_FILE_ATTRIBUTES && !(attributes & FILE_ATTRIBUTE_DIRECTORY));
}

void MainWindow::getTempFilename()
{
	WCHAR tempPath[MAX_PATH];
	GetTempPath(MAX_PATH, tempPath);
	WCHAR tempFilenameWchar[MAX_PATH];
	GetTempFileName(tempPath, L"WAV", 0, tempFilenameWchar);
	tempFilename = wstrtostr(tempFilenameWchar);
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
		try
		{
			if (!IsDialogMessage(dialog, &message))
			{
				TranslateMessage(&message);
				DispatchMessage(&message);
			}
		}
		catch (Exception &exception)
		{
			displayException(exception);
			return false;
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

	for (DWORD i = 0; i < 1; i++)
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

	selectChat(NULL);
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
	ListView_InsertItem(GetDlgItem(dialog, IDC_MAIN_CHATS), &item);

	delete[] text;
}

void MainWindow::selectChat(WhatsappChat *chat)
{
	SendDlgItemMessage(dialog, IDC_MAIN_MESSAGES, WM_CHATCONTROL_SETCHAT, 0, reinterpret_cast<LPARAM>(chat));
	SetWindowLongPtr(GetDlgItem(dialog, IDC_MAIN_EXPORT), GWLP_USERDATA, reinterpret_cast<LPARAM>(chat));
	EnableWindow(GetDlgItem(dialog, IDC_MAIN_EXPORT), chat != NULL);
}

void MainWindow::resizeChildWindows(int width, int height)
{
	int border = 15;
	int chatsWidth = 300;
	int buttonRowHeight = 25;

	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_CHATS), NULL, border, border, chatsWidth, height - border * 2, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_MESSAGES), NULL, chatsWidth + border * 2, border, width - chatsWidth - border * 3, height - border * 3 - buttonRowHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_EXPORT), NULL, chatsWidth + border * 2, height - border - buttonRowHeight, 150, buttonRowHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void MainWindow::clearChats()
{
	clearVector(chats);
}

bool isUncryptedWhatsappDatabase(const std::string &filename)
{
	std::ifstream file(filename.c_str(), std::ios_base::in | std::ios_base::binary);
	if (!file)
	{
		return false;
	}

	const char expectedBytes[] = "SQLite format 3";
	const int length = sizeof(expectedBytes);
	char bytes[length];

	file.read(bytes, length);

	return memcmp(bytes, expectedBytes, length) == 0;
}

void MainWindow::openDatabase()
{
	OpenDatabaseStruct openDatabaseStruct = lastDatabaseOpened;
	if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPEN_FILE), dialog, openDatabaseCallback, reinterpret_cast<LPARAM>(&openDatabaseStruct)) == IDOK)
	{
		closeDatabase();

		try
		{
			const std::string *filename = &openDatabaseStruct.filename;

			if (!isUncryptedWhatsappDatabase(*filename))
			{
				unsigned char key[24];
				buildKey(key, openDatabaseStruct.accountName);

				decryptWhatsappDatabase(*filename, tempFilename, key);
				filename = &tempFilename;
			}

			lastDatabaseOpened = openDatabaseStruct;
			settings.write("lastOpenedFile", lastDatabaseOpened.filename);
			settings.write("lastOpenedAccount", lastDatabaseOpened.accountName);

			database = new WhatsappDatabase(*filename);
			database->getChats(chats);
		}
		catch (Exception &exception)
		{
			displayException(exception);
		}

		addChats();
	}
}

void MainWindow::closeDatabase()
{
	clearChats();

	delete database;
	database = NULL;
}

void MainWindow::exportChat(WhatsappChat &chat)
{
	ChatExporter exporter(chat);
	exporter.exportChat("chat.txt");
	MessageBox(dialog, L"Chat exported to file chat.txt", L"Success", MB_OK | MB_ICONINFORMATION);
}

void MainWindow::decryptDatabase()
{
	OpenDatabaseStruct openDatabaseStruct = lastDatabaseOpened;
	if (DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_OPEN_FILE), dialog, openDatabaseCallback, reinterpret_cast<LPARAM>(&openDatabaseStruct)) == IDOK)
	{
		try
		{
			unsigned char key[24];
			buildKey(key, openDatabaseStruct.accountName);

			decryptWhatsappDatabase(openDatabaseStruct.filename, "msgstore.decrypted.db", key);

			lastDatabaseOpened = openDatabaseStruct;
			settings.write("lastOpenedFile", lastDatabaseOpened.filename);
			settings.write("lastOpenedAccount", lastDatabaseOpened.accountName);

			MessageBox(dialog, L"Database decrypted to file msgstore.decrypted.db", L"Success", MB_OK | MB_ICONINFORMATION);
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
				case ID_MENU_MAIN_FILE_DECRYPT:
				{
					mainWindow->decryptDatabase();
				} break;
				case IDC_MAIN_EXPORT:
				{
					WhatsappChat *chat = reinterpret_cast<WhatsappChat *>(GetWindowLongPtr(GetDlgItem(dialog, IDC_MAIN_EXPORT), GWLP_USERDATA));
					mainWindow->exportChat(*chat);
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
								mainWindow->selectChat(&chat);
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
