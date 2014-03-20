#include <windows.h>
#include <vector>

#include "ChatControl.h"
#include "../StringHelper.h"
#include "../../../WhatsApp/Chat.h"
#include "../../../WhatsApp/Message.h"

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

int drawMessage(WhatsappMessage &message, HDC deviceContext, int y, int clientRectWidth)
{
	int gap = 40;
	int left = 10;
	int right = clientRectWidth - gap - 10;
	int color = RGB(230, 230, 240);

	if (message.isFromMe())
	{
		left = 10 + gap;
		right = clientRectWidth - 10;
		color = RGB(190, 240, 150);
	}

	if (message.getData().find("cover") > 0)
	{
		left = left;
	}

	WCHAR *wcharText = buildWcharString(message.getData());

	SetBkColor(deviceContext, color);
	RECT textRect = { left, y, right, y };
	DrawText(deviceContext, wcharText, -1, &textRect, DT_CALCRECT | DT_WORDBREAK);

	textRect.left = left;
	textRect.right = right;

	HBRUSH brush = CreateSolidBrush(color);
	FillRect(deviceContext, &textRect, brush);
	DeleteObject(brush);

	DrawText(deviceContext, wcharText, -1, &textRect, DT_WORDBREAK);

	delete[] wcharText;

	return textRect.bottom - textRect.top;
}

LRESULT onPaint(ChatControl &chatControl, WPARAM wParam, LPARAM lParam)
{
	HDC deviceContext;
	PAINTSTRUCT paint;

	deviceContext = BeginPaint(chatControl.window, &paint);
	HANDLE oldFont = SelectObject(deviceContext, GetStockObject(DEFAULT_GUI_FONT));
	SetTextColor(deviceContext, RGB(0, 0, 0));
	SetBkColor(deviceContext, RGB(230, 230, 210));

	RECT clientRect;
	GetClientRect(chatControl.window, &clientRect);

	std::string text = "WhatsApp Chat";
	if (chatControl.chat != NULL)
	{
		text += " (";
		text += chatControl.chat->getKey();

		if (chatControl.chat->getSubject().length() > 0)
		{
			text += "; ";
			text += chatControl.chat->getSubject();
		}

		text += ")";
	}

	WCHAR *wcharText = buildWcharString(text);
	ExtTextOut(deviceContext, 10, 10, ETO_OPAQUE, &clientRect, wcharText, text.length(), NULL);
	delete[] wcharText;

	int y = 25;

	MoveToEx(deviceContext, 10, y, NULL);
	LineTo(deviceContext, clientRect.right - 10, y);

	y += 15;

	if (chatControl.chat != NULL)
	{
		WhatsappChat &chat = *chatControl.chat;
		std::vector<WhatsappMessage *> &messages = chat.getMessages();

		for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
		{
			WhatsappMessage &message = **it;

			y += drawMessage(message, deviceContext, y, clientRect.right);
			y += 8;

			if (y > clientRect.bottom)
			{
				break;
			}
		}
	}

	SelectObject(deviceContext, oldFont);
	EndPaint(chatControl.window, &paint);

	return 0;
}

void redraw(ChatControl &chatControl)
{
	InvalidateRect(chatControl.window, NULL, FALSE);
	UpdateWindow(chatControl.window);
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
			redraw(*chatControl);
		} break;
		case WM_PAINT:
		{
			return onPaint(*chatControl, wParam, lParam);
		} break;
		case WM_ERASEBKGND:
		{
			return 1;
		} break;
		case WM_NCDESTROY:
		{
			delete chatControl;
		} break;
	}

	return DefWindowProc(window, message, wParam, lParam);
}
