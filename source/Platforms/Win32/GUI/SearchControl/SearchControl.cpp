#include <windows.h>
#include <commctrl.h>

#include "SearchControl.h"
#include "../../../../Exceptions/Exception.h"
#include "../../StringHelper.h"
#include "../../ImageDecoder.h"
#include "../../Objects/Bitmap.h"
#include "../../../../../resources/resource.h"

WNDPROC buttonProc;
enum ButtonState
{
	BUTTON_NORMAL,
	BUTTON_OVER
};

bool mouseTracking = false;
ButtonState buttonState = BUTTON_NORMAL;

// TODO: merge with ChatControl.cpp
void renderBitmap2(HDC deviceContext, HBITMAP bitmapHandle, int x, int y);

SearchControl::SearchControl(HWND window)
	: window(window)
{
	ImageDecoder imageDecoder;
	searchIcon = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_SEARCH), L"PNG"));
	clearIcon = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_CLEAR), L"PNG"));
	clearIconHover = new Bitmap(imageDecoder.loadImageFromResource(MAKEINTRESOURCE(IDB_CLEAR_HOVER), L"PNG"));
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

	clearButton = CreateWindowEx(0, L"BUTTON", NULL, WS_VISIBLE | WS_CHILD | BS_OWNERDRAW, clientRect.right - 21, (clientRect.bottom - 16) / 2, 16, 16, window, NULL, GetModuleHandle(NULL), NULL);
	buttonProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(clearButton, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(clearButtonCallback))); 
}

void SearchControl::resizeChildWindows()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	SetWindowPos(editControl, NULL, 24, 2, clientRect.right - 48, clientRect.bottom - 2, SWP_NOZORDER | SWP_SHOWWINDOW);
	SetWindowPos(clearButton, NULL, clientRect.right - 24, 0, 24, clientRect.bottom, SWP_NOZORDER | SWP_SHOWWINDOW);
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
	EndPaint(window, &paint);
}

void SearchControl::onPaintClearButton(HDC deviceContext)
{
	RECT clientRect;
	GetClientRect(clearButton, &clientRect);

	Bitmap *bitmap = clearIcon;
	if (buttonState == BUTTON_OVER)
	{
		bitmap = clearIconHover;
	}

	renderBitmap2(deviceContext, bitmap->get(), (clientRect.right - 16) / 2, (clientRect.bottom - 16) / 2);
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
				if (drawItemStruct->hwndItem == control->clearButton)
				{
					control->onPaintClearButton(drawItemStruct->hDC);
					return TRUE;
				}
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

void onMouseEnter(HWND window)
{
	mouseTracking = true;
	buttonState = BUTTON_OVER;

	SetCapture(window);
	InvalidateRect(window, NULL, TRUE);
	UpdateWindow(window);
}

void onMouseLeave(HWND window)
{
	buttonState = BUTTON_NORMAL;
	mouseTracking = false;

	InvalidateRect(window, NULL, TRUE);
	UpdateWindow(window);
	ReleaseCapture();
}

LRESULT CALLBACK clearButtonCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_MOUSEMOVE:
		{
			POINT point;
			point.x = LOWORD(lParam);
			point.y = HIWORD(lParam);

			if (!mouseTracking || GetCapture() != window)
			{
				onMouseEnter(window);
			}
			else
			{
				if (buttonState == BUTTON_OVER)
				{
					RECT clientRect;
					GetClientRect(window, &clientRect);
					if (!PtInRect(&clientRect, point))
					{
						onMouseLeave(window);
					}
				}
			}
		} break;
		case WM_LBUTTONUP:
		{
			buttonState = BUTTON_OVER;
		} break;
		case WM_CAPTURECHANGED:
		{
			mouseTracking = false;
		} break;
	}

	return CallWindowProc(buttonProc, window, message, wParam, lParam);
}
