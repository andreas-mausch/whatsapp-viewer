#include <windows.h>

#include "ChatControl.h"

void registerChatControl()
{
	WNDCLASSEX windowClass;
	memset(&windowClass, 0, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpszClassName = L"ChatControl";
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	windowClass.style = 0;
	windowClass.lpfnWndProc = ChatControlCallback;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.cbWndExtra = sizeof(ChatControl *);
	RegisterClassEx(&windowClass);
}

LRESULT onPaint(ChatControl &chatControl, WPARAM wParam, LPARAM lParam)
{
	HDC deviceContext;
	PAINTSTRUCT paint;

	deviceContext = BeginPaint(chatControl.window, &paint);
	HANDLE oldFont = SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
	SetTextColor(deviceContext, RGB(0, 0, 0));
	SetBkColor(deviceContext, RGB(255, 240, 220));

	RECT rect;
	GetClientRect(chatControl.window, &rect);

	ExtTextOut(deviceContext, 10, 10, ETO_OPAQUE, &rect, L"Test", 4, NULL);

	SelectObject(deviceContext, oldFont);
	EndPaint(chatControl.window, &paint);

	return 0;
}

LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ChatControl *chatControl = reinterpret_cast<ChatControl *>(GetWindowLongPtr(window, 0));

	switch (message)
	{
		case WM_NCCREATE:
		{
			chatControl = new ChatControl;
			chatControl->window = window;
			chatControl->chat = NULL;

			SetWindowLongPtr(window, 0, reinterpret_cast<LONG>(chatControl));
		} break;
		case WM_CHATCONTROL_SETCHAT:
		{
			chatControl->chat = reinterpret_cast<WhatsappChat *>(lParam);
		} break;
		case WM_PAINT:
		{
			return onPaint(*chatControl, wParam, lParam);
		} break;
		case WM_NCDESTROY:
		{
			delete chatControl;
		} break;
	}

	return DefWindowProc(window, message, wParam, lParam);
}
