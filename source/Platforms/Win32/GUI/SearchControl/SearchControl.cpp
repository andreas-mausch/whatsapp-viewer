#include <windows.h>

#include "SearchControl.h"
#include "../../../../Exceptions/Exception.h"
#include "../../StringHelper.h"
#include "../../ImageDecoder.h"
#include "../../Objects/Bitmap.h"
#include "../../../../../resources/resource.h"

// TODO: merge with ChatControl.cpp
void renderBitmap2(HDC deviceContext, HBITMAP bitmapHandle, int x, int y);

SearchControl::SearchControl(HWND window)
	: window(window)
{
	ImageDecoder imageDecoder;
	searchIcon = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_SEARCH), L"PNG"));
	clearIcon = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_CLEAR), L"PNG"));
}

SearchControl::~SearchControl()
{
	delete clearIcon;
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
}

void SearchControl::resizeChildWindows()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	SetWindowPos(editControl, NULL, 24, 2, clientRect.right - 48, clientRect.bottom - 2, SWP_NOZORDER | SWP_SHOWWINDOW);
}

void SearchControl::show(bool visible)
{
	int code = (visible) ? SW_SHOW : SW_HIDE;
	ShowWindow(window, code);
}

void SearchControl::onPaint()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	PAINTSTRUCT paint;
	HDC deviceContext = BeginPaint(window, &paint);
	renderBitmap2(deviceContext, searchIcon->get(), 3, (clientRect.bottom - 16) / 2);
	renderBitmap2(deviceContext, clearIcon->get(), clientRect.right - 21, (clientRect.bottom - 16) / 2);
	EndPaint(window, &paint);
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
