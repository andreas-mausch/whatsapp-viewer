#include <windows.h>
#include <commctrl.h>

#include "SearchControl.h"
#include "HoverButton.h"
#include "../../../../Exceptions/Exception.h"
#include "../../StringHelper.h"
#include "../../ImageDecoder.h"
#include "../../Objects/Bitmap.h"
#include "../../../../../resources/resource.h"

// TODO: merge with ChatControl.cpp
void renderBitmap2(HDC deviceContext, HBITMAP bitmapHandle, int x, int y);

SearchControl::SearchControl(HWND window)
	: window(window), clearButton(NULL)
{
	ImageDecoder imageDecoder;
	searchIcon = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_SEARCH), L"PNG"));
	clearIcon = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_CLEAR), L"PNG"));
	clearIconHover = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_CLEAR_HOVER), L"PNG"));
}

SearchControl::~SearchControl()
{
	delete clearButton;
	delete clearIcon;
	delete clearIconHover;
	delete searchIcon;
}

void SearchControl::registerControl()
{
	WNDCLASSEX windowClass;
	memset(&windowClass, 0, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpszClassName = L"SearchControl";
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	windowClass.style = 0;
	windowClass.lpfnWndProc = callback;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.cbWndExtra = sizeof(SearchControl *);
	RegisterClassEx(&windowClass);
}

void SearchControl::onCreate()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	editControl = CreateWindowEx(0, L"EDIT", NULL, WS_VISIBLE | WS_CHILD | ES_LEFT, 24, 2, clientRect.right - 48, clientRect.bottom - 2, window, NULL, GetModuleHandle(NULL), NULL);
	SendMessage(editControl, WM_SETFONT, reinterpret_cast<WPARAM>(GetStockObject(DEFAULT_GUI_FONT)), 0);

	HWND clearButtonHandle = CreateWindowEx(0, L"BUTTON", NULL, WS_CHILD | BS_OWNERDRAW, clientRect.right - 21, (clientRect.bottom - 16) / 2, 16, 16, window, NULL, GetModuleHandle(NULL), NULL);
	clearButton = new HoverButton(clearButtonHandle, *clearIcon, *clearIconHover);
}

void SearchControl::resizeChildWindows()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	SetWindowPos(editControl, NULL, 24, 2, clientRect.right - 48, clientRect.bottom - 2, SWP_NOZORDER);
	SetWindowPos(clearButton->getHandle(), NULL, clientRect.right - 24, 0, 24, clientRect.bottom, SWP_NOZORDER);
}

void SearchControl::showClearButton(const WCHAR *textW)
{
	std::string text = wstrtostr(textW);
	bool show = false;
	if (text.size() > 0)
	{
		show = true;
	}

	clearButton->show(show);
}

void SearchControl::onPaint()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	PAINTSTRUCT paint;
	HDC deviceContext = BeginPaint(window, &paint);
	renderBitmap2(deviceContext, searchIcon->get(), 3, (clientRect.bottom - 16) / 2);
	EndPaint(window, &paint);
}

void SearchControl::onTextChange()
{
	int id = GetWindowLong(window, GWL_ID);
	HWND parent = GetParent(window);

	WCHAR text[256];
	GetWindowText(editControl, text, 256);
	SetWindowText(window, text);

	showClearButton(text);
	SendMessage(parent, WM_COMMAND, MAKEWPARAM(id, EN_CHANGE), reinterpret_cast<LPARAM>(window));
}

void SearchControl::onClear()
{
	SetWindowText(window, L"");
	SetWindowText(editControl, L"");
}

LRESULT CALLBACK SearchControl::callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	SearchControl *control = reinterpret_cast<SearchControl *>(GetWindowLongPtr(window, 0));

	try
	{
		switch (message)
		{
			case WM_NCCREATE:
			{
				try
				{
					control = new SearchControl(window);
				}
				catch (Exception &exception)
				{
					std::wstring cause = strtowstr(exception.getCause());
					MessageBox(NULL, cause.c_str(), L"Error", MB_OK | MB_ICONERROR);
					return FALSE;
				}

				SetWindowLongPtr(window, 0, reinterpret_cast<LONG>(control));
			} break;
			case WM_NCDESTROY:
			{
				delete control;
			} break;
			case WM_CREATE:
			{
				control->onCreate();
				return 0;
			} break;
			case WM_PAINT:
			{
				control->onPaint();
				return 0;
			} break;
			case WM_SIZE:
			{
				control->resizeChildWindows();
			} break;
			case WM_CTLCOLORBTN:
			{
				HWND button = reinterpret_cast<HWND>(lParam);
				return reinterpret_cast<LRESULT>(GetStockObject(NULL_BRUSH));
			} break;
			case WM_DRAWITEM:
			{
				DRAWITEMSTRUCT *drawItemStruct = reinterpret_cast<DRAWITEMSTRUCT *>(lParam);
				if (drawItemStruct->hwndItem == control->clearButton->getHandle())
				{
					control->clearButton->onPaint(drawItemStruct->hDC);
					return TRUE;
				}
			} break;
			case WM_COMMAND:
			{
				switch (HIWORD(wParam))
				{
					case BN_CLICKED:
					{
						HWND button = reinterpret_cast<HWND>(lParam);
						if (button == control->clearButton->getHandle())
						{
							control->onClear();
						}
					} break;
					case EN_CHANGE:
					{
						HWND editControl = reinterpret_cast<HWND>(lParam);
						if (editControl == control->editControl)
						{
							control->onTextChange();
						}
					} break;
				}
			} break;
			case WM_SEARCHCONTROL:
			{
				switch (wParam)
				{
					case SEARCHCONTROL_SETTEXT:
					{
						WCHAR *text = reinterpret_cast<WCHAR *>(lParam);
						SetWindowText(window, text);
						SetWindowText(control->editControl, text);
					} break;
				}
			}
		}
	}
	catch (Exception &exception)
	{
		std::wstring cause = strtowstr(exception.getCause());
		MessageBox(NULL, cause.c_str(), L"Error", MB_OK | MB_ICONERROR);
		return DefWindowProc(window, message, wParam, lParam);
	}

	return DefWindowProc(window, message, wParam, lParam);
}
