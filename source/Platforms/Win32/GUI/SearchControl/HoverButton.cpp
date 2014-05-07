#include <windows.h>

#include "HoverButton.h"
#include "../../Objects/Bitmap.h"

// TODO: merge with ChatControl.cpp
void renderBitmap2(HDC deviceContext, HBITMAP bitmapHandle, int x, int y);

HoverButton::HoverButton(HWND window, Bitmap &normal, Bitmap &hover)
	: window(window), normal(normal), hover(hover),
	mouseTracking(false), state(BUTTON_NORMAL)
{
	buttonProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(window, GWL_WNDPROC, reinterpret_cast<LONG_PTR>(callback))); 
	SetProp(window, L"HOVER_BUTTON_POINTER", this);
}

HoverButton::~HoverButton()
{
}

void HoverButton::onMouseMove(POINT &point)
{
	if (!mouseTracking || GetCapture() != window)
	{
		onMouseEnter();
	}
	else
	{
		if (state == BUTTON_OVER)
		{
			RECT clientRect;
			GetClientRect(window, &clientRect);
			if (!PtInRect(&clientRect, point))
			{
				onMouseLeave();
			}
		}
	}
}

void HoverButton::onMouseEnter()
{
	mouseTracking = true;
	state = BUTTON_OVER;

	SetCapture(window);
	InvalidateRect(window, NULL, TRUE);
	UpdateWindow(window);
}

void HoverButton::onMouseLeave()
{
	mouseTracking = false;
	state = BUTTON_NORMAL;

	InvalidateRect(window, NULL, TRUE);
	UpdateWindow(window);
	ReleaseCapture();
}

void HoverButton::onPaint(HDC deviceContext)
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	if (!IsWindowVisible(window))
	{
		// TODO: edit control is not guaranteed to be white
		FillRect(deviceContext, &clientRect, reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH)));
		return;
	}

	Bitmap *bitmap = &normal;
	if (state == BUTTON_OVER)
	{
		bitmap = &hover;
	}

	renderBitmap2(deviceContext, bitmap->get(), (clientRect.right - 16) / 2, (clientRect.bottom - 16) / 2);
}

HWND HoverButton::getHandle()
{
	return window;
}

void HoverButton::show(bool visible)
{
	ShowWindow(window, visible ? SW_SHOW : SW_HIDE);

	if (!visible)
	{
		onMouseLeave();
	}
}

LRESULT CALLBACK HoverButton::callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	HoverButton *hoverButton = reinterpret_cast<HoverButton *>(GetProp(window, L"HOVER_BUTTON_POINTER"));

	switch(message)
	{
		case WM_MOUSEMOVE:
		{
			if (hoverButton)
			{
				POINT point;
				point.x = LOWORD(lParam);
				point.y = HIWORD(lParam);

				hoverButton->onMouseMove(point);
			}
		} break;
		case WM_CAPTURECHANGED:
		{
			if (hoverButton)
			{
				hoverButton->mouseTracking = false;
			}
		} break;
	}

	if (hoverButton)
	{
		return CallWindowProc(hoverButton->buttonProc, window, message, wParam, lParam);
	}

	return 0;
}
