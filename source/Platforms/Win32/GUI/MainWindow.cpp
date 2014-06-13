#include <windows.h>
#include <commctrl.h>
#include <algorithm>
#include <fstream>
#include <vector>

#include "MainWindow.h"
#include "AboutDialog.h"
#include "OpenDatabaseDialog.h"
#include "DecryptDatabaseDialog5.h"
#include "DecryptDatabaseDialog7.h"
#include "ChatControl/ChatControl.h"
#include "SearchControl/SearchControl.h"
#include "../Objects/Bitmap.h"
#include "../../../ChatExporter.h"
#include "../../../WhatsAppViewer.h"
#include "../../../../resources/resource.h"
#include "../../../Exceptions/Exception.h"
#include "../../../WhatsApp/Chat.h"
#include "../../../WhatsApp/Contacts.h"
#include "../../../WhatsApp/Crypt5.h"
#include "../../../WhatsApp/Crypt7.h"
#include "../../../WhatsApp/Database.h"
#include "../../../WhatsApp/Message.h"
#include "../../../VectorUtils.h"
#include "../ImageDecoder.h"
#include "../StringHelper.h"
#include "../Timestamp.h"

#pragma comment(linker, \
 "\"/manifestdependency:type='Win32' "\
 "name='Microsoft.Windows.Common-Controls' "\
 "version='6.0.0.0' "\
 "processorArchitecture='*' "\
 "publicKeyToken='6595b64144ccf1df' "\
 "language='*'\"")

MainWindow::MainWindow(WhatsAppViewer &whatsAppViewer, ImageDecoder &imageDecoder)
	: whatsAppViewer(whatsAppViewer), imageDecoder(imageDecoder),
	settings(whatsAppViewer.getSettings()), database(NULL),
	sortingColumn(1), sortingDirection(SORTING_DIRECTION_DESCENDING),
	dialog(NULL), accelerator(MAKEINTRESOURCE(IDR_ACCELERATOR)), aboutDialog(NULL)
{
	INITCOMMONCONTROLSEX icex;
	icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
	icex.dwICC = ICC_LISTVIEW_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS | ICC_STANDARD_CLASSES | ICC_TAB_CLASSES | ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icex);

	ChatControl::registerControl();
	SearchControl::registerControl();

	getTempFilename();

	readSettings();

	if (!CreateDialogParam(GetModuleHandle(NULL),
						   MAKEINTRESOURCE(IDD_MAIN),
						   NULL,
						   dialogCallback,
						   reinterpret_cast<LPARAM>(this)))
	{
		throw Exception("could not create main dialog");
	}

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
		lastDatabaseOpened.keyFilename = settings.read("lastOpenedKeyfile");
		lastDatabaseOpened.wadbFilename = settings.read("lastOpenedWadbfile");
	}
	catch (Exception &)
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
		if (!TranslateAccelerator(dialog, accelerator.get(), &message))
		{
			if (!IsDialogMessage(dialog, &message))
			{
				if (!aboutDialog || !IsDialogMessage(aboutDialog->getHandle(), &message))
				{
					TranslateMessage(&message);
					DispatchMessage(&message);
				}
			}
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
	DWORD columnsWidths[] = { 220, 140 };

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

void MainWindow::clearChats()
{
	clearChatList();
	clearVector(chats);
}

void MainWindow::clearChatList()
{
	ListView_DeleteAllItems(GetDlgItem(dialog, IDC_MAIN_CHATS));
	selectChat(NULL);
}

void MainWindow::addChats()
{
	clearChatList();

	for (std::vector<WhatsappChat *>::iterator it = chats.begin(); it != chats.end(); ++it)
	{
		addChat(**it);
	}

	sortChats();
}

void MainWindow::addChat(WhatsappChat &chat)
{
	std::wstring text = strtowstr(chat.getDisplayName());
	std::wstring lastMessageText = strtowstr(formatTimestamp(chat.getLastMessage()));

	LVITEM item;
	ZeroMemory(&item, sizeof(LVITEM));

	item.iItem = ListView_GetItemCount(GetDlgItem(dialog, IDC_MAIN_CHATS));
	item.mask = LVIF_TEXT | LVIF_PARAM;
	item.pszText = const_cast<WCHAR *>(text.c_str());
	item.lParam = reinterpret_cast<LPARAM>(&chat);
	ListView_InsertItem(GetDlgItem(dialog, IDC_MAIN_CHATS), &item);

	ListView_SetItemText(GetDlgItem(dialog, IDC_MAIN_CHATS), item.iItem, 1, const_cast<WCHAR *>(lastMessageText.c_str()));
}

void MainWindow::selectChat(WhatsappChat *chat)
{
	SendDlgItemMessage(dialog, IDC_MAIN_MESSAGES, WM_CHATCONTROL, CHAT_CONTROL_SETCHAT, reinterpret_cast<LPARAM>(chat));
	SetWindowLongPtr(GetDlgItem(dialog, IDC_MAIN_EXPORT), GWLP_USERDATA, reinterpret_cast<LPARAM>(chat));
	EnableWindow(GetDlgItem(dialog, IDC_MAIN_EXPORT), chat != NULL);
}

void MainWindow::resizeChildWindows(int width, int height)
{
	int border = 15;
	int searchBoxHeight = 22;
	int chatsWidth = 400;
	int buttonRowHeight = 25;

	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_SEARCH_CHATS), NULL, border, border, chatsWidth, searchBoxHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_CHATS), NULL, border, border + searchBoxHeight + 5, chatsWidth, height - border * 2 - searchBoxHeight - 5, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_MESSAGES), NULL, chatsWidth + border * 2, border, width - chatsWidth - border * 3, height - border * 3 - buttonRowHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(GetDlgItem(dialog, IDC_MAIN_EXPORT), NULL, chatsWidth + border * 2, height - border - buttonRowHeight, 150, buttonRowHeight, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void MainWindow::sortChats()
{
	ListView_SortItems(GetDlgItem(dialog, IDC_MAIN_CHATS), sortingCallback, reinterpret_cast<LPARAM>(this));
	updateSortingArrow();
}

int CALLBACK MainWindow::sortingCallback(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	WhatsappChat *chat1 = reinterpret_cast<WhatsappChat *>(lParam1);
	WhatsappChat *chat2 = reinterpret_cast<WhatsappChat *>(lParam2);
	MainWindow *mainWindow = reinterpret_cast<MainWindow *>(lParamSort);

	int result = 0;

	switch (mainWindow->sortingColumn)
	{
		case 0:
		{
			// key
			result = chat1->getKey().compare(chat2->getKey());
		} break;
		case 1:
		{
			// timestamp
			if (chat1->getLastMessage() == chat2->getLastMessage())
			{
				result = 0;
			}
			else if (chat1->getLastMessage() < chat2->getLastMessage())
			{
				result = -1;
			}
			else
			{
				result = 1;
			}
		} break;
	}

	if (mainWindow->sortingDirection == SORTING_DIRECTION_ASCENDING)
	{
		return result;
	}
	else
	{
		return -result;
	}
}

void MainWindow::updateSortingArrow()
{
	ListViewShowArrow arrow = LISTVIEW_SHOW_UP_ARROW;
	if (sortingDirection == SORTING_DIRECTION_DESCENDING)
	{
		arrow = LISTVIEW_SHOW_DOWN_ARROW;
	}
	xListViewSetSortArrow(GetDlgItem(dialog, IDC_MAIN_CHATS), 0, LISTVIEW_SHOW_NO_ARROW);
	xListViewSetSortArrow(GetDlgItem(dialog, IDC_MAIN_CHATS), 1, LISTVIEW_SHOW_NO_ARROW);
	xListViewSetSortArrow(GetDlgItem(dialog, IDC_MAIN_CHATS), sortingColumn, arrow);
}

void MainWindow::setSortingColumn(int columnIndex)
{
	if (sortingColumn == columnIndex)
	{
		if (sortingDirection == SORTING_DIRECTION_ASCENDING)
		{
			sortingDirection = SORTING_DIRECTION_DESCENDING;
		}
		else
		{
			sortingDirection = SORTING_DIRECTION_ASCENDING;
		}
	}
	else
	{
		sortingColumn = columnIndex;
		sortingDirection = SORTING_DIRECTION_ASCENDING;
	}

	sortChats();
}

bool isPlainWhatsappDatabase(const std::string &filename)
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
	OpenDatabaseDialog dialog(MainWindow::dialog, openDatabaseStruct);

	if (dialog.openModal() == IDOK)
	{
		closeDatabase();
		clearChatList();

		try
		{
			if (openDatabaseStruct.wadbFilename.length() > 0)
			{
				importContacts(settings, openDatabaseStruct.wadbFilename);
			}

			const std::string *filename = &openDatabaseStruct.filename;

			if (!isPlainWhatsappDatabase(*filename))
			{
				unsigned char key[24];
				buildKey(key, openDatabaseStruct.accountName);

				decryptWhatsappDatabase5(*filename, tempFilename, key);
				filename = &tempFilename;
			}

			lastDatabaseOpened = openDatabaseStruct;
			settings.write("lastOpenedFile", lastDatabaseOpened.filename);
			settings.write("lastOpenedAccount", lastDatabaseOpened.accountName);
			settings.write("lastOpenedWadbfile", lastDatabaseOpened.wadbFilename);

			openPlainDatabase(*filename);
		}
		catch (Exception &exception)
		{
			displayException(MainWindow::dialog, exception);
		}
	}
}

void MainWindow::openDatabase(const std::string &filename)
{
	if (isPlainWhatsappDatabase(filename))
	{
		openPlainDatabase(filename);
		lastDatabaseOpened.filename = filename;
		settings.write("lastOpenedFile", filename);
	}
	else
	{
		lastDatabaseOpened.filename = filename;
		openDatabase();
	}
}

void MainWindow::openPlainDatabase(const std::string &filename)
{
	closeDatabase();
	SendDlgItemMessage(dialog, IDC_MAIN_SEARCH_CHATS, WM_SEARCHCONTROL, SEARCHCONTROL_SETTEXT, reinterpret_cast<LPARAM>(L""));

	lastDatabaseOpened.filename = filename;

	database = new WhatsappDatabase(filename);
	database->getChats(settings, chats);

	addChats();
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

void MainWindow::decryptDatabaseCrypt5()
{
	OpenDatabaseStruct openDatabaseStruct = lastDatabaseOpened;
	DecryptDatabaseDialog5 dialog(MainWindow::dialog, openDatabaseStruct);

	if (dialog.openModal() == IDOK)
	{
		try
		{
			unsigned char key[24];
			buildKey(key, openDatabaseStruct.accountName);

			decryptWhatsappDatabase5(openDatabaseStruct.filename, "msgstore.decrypted.db", key);

			lastDatabaseOpened = openDatabaseStruct;
			settings.write("lastOpenedFile", lastDatabaseOpened.filename);
			settings.write("lastOpenedAccount", lastDatabaseOpened.accountName);

			MessageBox(MainWindow::dialog, L"Database decrypted to file msgstore.decrypted.db", L"Success", MB_OK | MB_ICONINFORMATION);
		}
		catch (Exception &exception)
		{
			displayException(MainWindow::dialog, exception);
		}
	}
}

void MainWindow::decryptDatabaseCrypt7()
{
	OpenDatabaseStruct openDatabaseStruct = lastDatabaseOpened;
	DecryptDatabaseDialog7 dialog(MainWindow::dialog, openDatabaseStruct);

	if (dialog.openModal() == IDOK)
	{
		try
		{
			decryptWhatsappDatabase7(openDatabaseStruct.filename, "msgstore.decrypted.db", openDatabaseStruct.keyFilename);

			lastDatabaseOpened = openDatabaseStruct;
			settings.write("lastOpenedFile", lastDatabaseOpened.filename);
			settings.write("lastOpenedKeyfile", lastDatabaseOpened.keyFilename);

			MessageBox(MainWindow::dialog, L"Database decrypted to file msgstore.decrypted.db", L"Success", MB_OK | MB_ICONINFORMATION);
		}
		catch (Exception &exception)
		{
			displayException(MainWindow::dialog, exception);
		}
	}
}

void MainWindow::displayException(HWND mainWindow, Exception &exception)
{
	std::wstring cause = strtowstr(exception.getCause());
	MessageBox(mainWindow, cause.c_str(), L"Error", MB_OK | MB_ICONERROR);
}

void MainWindow::showAboutDialog()
{
	if (aboutDialog == NULL)
	{
		aboutDialog = new AboutDialog(dialog);
		aboutDialog->open();
	}
}

void MainWindow::close()
{
	DestroyWindow(dialog);
}

void MainWindow::onDrop(HDROP drop)
{
	if (DragQueryFile(drop, -1, NULL, 0) == 1)
	{
		WCHAR filenameW[MAX_PATH];
		if (DragQueryFile(drop, 0, filenameW, MAX_PATH))
		{
			openDatabase(wstrtostr(filenameW));
		}
	}
	else
	{
		MessageBox(dialog, L"You can only drop a single database file to this window.", L"Information", MB_OK | MB_ICONINFORMATION);
	}

	DragFinish(drop);
}

void MainWindow::searchChats()
{
	WCHAR searchPatternW[256];
	GetDlgItemText(dialog, IDC_MAIN_SEARCH_CHATS, searchPatternW, 256);
	std::string searchPattern = wstrtostr(searchPatternW);
	std::transform(searchPattern.begin(), searchPattern.end(), searchPattern.begin(), tolower);

	clearChatList();

	for (std::vector<WhatsappChat *>::iterator it = chats.begin(); it != chats.end(); ++it)
	{
		WhatsappChat *chat = *it;

		std::string displayName = chat->getDisplayName();
		std::transform(displayName.begin(), displayName.end(), displayName.begin(), tolower);

		if ((chat->getKey().find(searchPattern) != std::string::npos) ||
			(displayName.find(searchPattern) != std::string::npos))
		{
			addChat(*chat);
		}
	}

	sortChats();
}

INT_PTR MainWindow::handleMessage(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
		{
			// save the dialog handle of the dialog box
			this->dialog = dialog;

			createChildWindows();
			DragAcceptFiles(dialog, TRUE);
		} break;
		case WM_COMMAND:
		{
			switch (HIWORD(wParam))
			{
				case 1: // Accelerator
				{
					switch(LOWORD(wParam))
					{
						case ID_ACCELERATOR_OPEN:
						{
							openDatabase();
						} break;
						case ID_ACCELERATOR_DECRYPT_CRYPT5:
						{
							decryptDatabaseCrypt5();
						} break;
						case ID_ACCELERATOR_DECRYPT_CRYPT7:
						{
							decryptDatabaseCrypt7();
						} break;
					}
				} break;
				case BN_CLICKED:
				{
					switch(LOWORD(wParam))
					{
						case ID_MENU_MAIN_FILE_OPEN:
						{
							openDatabase();
						} break;
						case ID_MENU_MAIN_FILE_DECRYPT_CRYPT5:
						{
							decryptDatabaseCrypt5();
						} break;
						case ID_MENU_MAIN_FILE_DECRYPT_CRYPT7:
						{
							decryptDatabaseCrypt7();
						} break;
						case IDC_MAIN_EXPORT:
						{
							WhatsappChat *chat = reinterpret_cast<WhatsappChat *>(GetWindowLongPtr(GetDlgItem(dialog, IDC_MAIN_EXPORT), GWLP_USERDATA));
							exportChat(*chat);
						} break;
						case ID_MENU_MAIN_FILE_EXIT:
						{
							close();
						} break;
						case ID_MENU_MAIN_HELP_ABOUT:
						{
							showAboutDialog();
						} break;
					}
				} break;
				case EN_CHANGE:
				{
					switch(LOWORD(wParam))
					{
						case IDC_MAIN_SEARCH_CHATS:
						{
							searchChats();
						} break;
					}
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
								nmListView->uNewState & LVIS_SELECTED &&
								!(nmListView->uOldState & LVIS_SELECTED))
							{
								LVITEM item;
								ZeroMemory(&item, sizeof(LVITEM));
								item.mask = LVIF_PARAM;
								item.iItem = nmListView->iItem;
								ListView_GetItem(GetDlgItem(dialog, IDC_MAIN_CHATS), &item);

								WhatsappChat &chat = *reinterpret_cast<WhatsappChat *>(item.lParam);
								selectChat(&chat);
							}
						} break;
					}
				} break;
				case LVN_COLUMNCLICK:
				{
					NMLISTVIEW *nmListView = reinterpret_cast<NMLISTVIEW *>(lParam);
					setSortingColumn(nmListView->iSubItem);
				} break;
			}
		} break;
		case WM_DIALOG:
		{
			switch(LOWORD(wParam))
			{
				case DIALOG_CLOSED:
				{
					Dialog *dialog = reinterpret_cast<Dialog *>(lParam);
					if (dialog == aboutDialog)
					{
						delete aboutDialog;
						aboutDialog = NULL;
					}
				} break;
			}
		} break;
		case WM_DROPFILES:
		{
			HDROP drop = reinterpret_cast<HDROP>(wParam);
			onDrop(drop);
		} break;
		case WM_ENTERSIZEMOVE:
		{
			SendDlgItemMessage(dialog, IDC_MAIN_MESSAGES, WM_CHATCONTROL, CHAT_CONTROL_STOP_RESIZING_MESSAGES, 0);
		} break;
		case WM_EXITSIZEMOVE:
		{
			SendDlgItemMessage(dialog, IDC_MAIN_MESSAGES, WM_CHATCONTROL, CHAT_CONTROL_START_RESIZING_MESSAGES, 0);
			SendDlgItemMessage(dialog, IDC_MAIN_MESSAGES, WM_CHATCONTROL, CHAT_CONTROL_REDRAW, 0);
		} break;
		case WM_SIZE:
		{
			resizeChildWindows(LOWORD(lParam), HIWORD(lParam));
		} break;
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO *minmaxinfo = reinterpret_cast<MINMAXINFO *>(lParam);
			minmaxinfo->ptMinTrackSize.x = 600;
			minmaxinfo->ptMinTrackSize.y = 200;
		} break;
		case WM_CLOSE:
		{
			close();
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

INT_PTR MainWindow::dialogCallback(HWND dialog, UINT message, WPARAM wParam, LPARAM lParam)
{
	try
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
			} break;
		}

		if (mainWindow == NULL)
		{
			return 0;
		}

		return mainWindow->handleMessage(dialog, message, wParam, lParam);
	}
	catch (Exception &exception)
	{
		displayException(dialog, exception);
		return 0;
	}
}
