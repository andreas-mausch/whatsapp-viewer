#include <time.h>
#include <windows.h>
#include <commctrl.h>
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
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = ChatControlCallback;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.cbWndExtra = sizeof(ChatControl *);
	RegisterClassEx(&windowClass);
}

std::string timestampToString(long long timestamp)
{
    char buffer[60];
	tm date;
	timestamp /= 1000;
    localtime_s(&date, &timestamp);
	strftime(buffer, 60, "%Y.%m.%d - %H:%M:%S", &date);
    return buffer;
}

int drawMessage(WhatsappMessage &message, HDC deviceContext, int y, int clientRectWidth, HGDIOBJ dateFont)
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
	WCHAR *wcharDate = buildWcharString(timestampToString(message.getTimestamp()));

	SetBkColor(deviceContext, color);
	RECT textRect = { left, y, right, y };
	int height = 0;

	DrawText(deviceContext, wcharText, -1, &textRect, DT_CALCRECT | DT_WORDBREAK);
	height += textRect.bottom - textRect.top;

	RECT dateRect = { left, y + height, right, y + height };
	HGDIOBJ oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_CALCRECT | DT_WORDBREAK | DT_RIGHT);
	dateRect.right = right;
	SelectObject(deviceContext, oldFont);
	height += dateRect.bottom - dateRect.top;

	RECT completeRect = { left, y, right, y + height };

	HBRUSH brush = CreateSolidBrush(color);
	FillRect(deviceContext, &completeRect, brush);
	DeleteObject(brush);

	DrawText(deviceContext, wcharText, -1, &textRect, DT_WORDBREAK);

	oldFont = SelectObject(deviceContext, dateFont);
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_WORDBREAK | DT_RIGHT);
	SelectObject(deviceContext, oldFont);

	delete[] wcharText;
	delete[] wcharDate;

	return completeRect.bottom - completeRect.top;
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

	int scrollPosition = GetScrollPos(chatControl.window, SB_VERT);

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
	ExtTextOut(deviceContext, 10, 10 - scrollPosition, ETO_OPAQUE, &clientRect, wcharText, text.length(), NULL);
	delete[] wcharText;

	int y = 25;

	MoveToEx(deviceContext, 10, y - scrollPosition, NULL);
	LineTo(deviceContext, clientRect.right - 10, y - scrollPosition);

	y += 15;

	if (chatControl.chat != NULL)
	{
		WhatsappChat &chat = *chatControl.chat;
		std::vector<WhatsappMessage *> &messages = chat.getMessages();

		for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
		{
			WhatsappMessage &message = **it;

			y += drawMessage(message, deviceContext, y - scrollPosition, clientRect.right, chatControl.dateFont);
			y += 8;

			if (y > clientRect.bottom)
			{
				if (chatControl.chatSizeY != 0)
				{
					break;
				}
			}
		}

		if (chatControl.chatSizeY == 0)
		{
			SCROLLINFO scrollInfo;
			memset(&scrollInfo, 0, sizeof(SCROLLINFO));
			scrollInfo.cbSize = sizeof(SCROLLINFO);
			scrollInfo.fMask = SIF_PAGE | SIF_RANGE;
			scrollInfo.nMin = 0;
			scrollInfo.nMax = y;
			scrollInfo.nPage = clientRect.bottom;

			SetScrollInfo(chatControl.window, SB_VERT, &scrollInfo, TRUE);
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

LRESULT onScroll(ChatControl &chatControl, WPARAM wParam)
{
	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(chatControl.window, SB_VERT, &scrollInfo);

	int previousPosition = scrollInfo.nPos;

	switch (LOWORD(wParam))
	{
		case SB_TOP:
		{
			scrollInfo.nPos = scrollInfo.nMin;
		} break;
		case SB_BOTTOM:
		{
			scrollInfo.nPos = scrollInfo.nMax;
		};
		case SB_LINEUP:
		{
			scrollInfo.nPos -= 30;
		} break;
		case SB_LINEDOWN:
		{
			scrollInfo.nPos += 30;
		} break;
		case SB_PAGEUP:
		{
			scrollInfo.nPos -= scrollInfo.nPage;
		} break;
		case SB_PAGEDOWN:
		{
			scrollInfo.nPos += scrollInfo.nPage;
		} break;
		case SB_THUMBTRACK:
		{
			scrollInfo.nPos = scrollInfo.nTrackPos;
		} break;
	}

	scrollInfo.fMask = SIF_POS;
	SetScrollInfo(chatControl.window, SB_VERT, &scrollInfo, TRUE);
	GetScrollInfo(chatControl.window, SB_VERT, &scrollInfo);

	if (scrollInfo.nPos != previousPosition)
	{
		// ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
		// UpdateWindow (hwnd);
		redraw(chatControl);
	}

	return 0;
}

LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ChatControl *chatControl = reinterpret_cast<ChatControl *>(GetWindowLongPtr(window, 0));

	switch (message)
	{
		case WM_NCCREATE:
		{
			RECT rect;
			GetClientRect(window, &rect);

			chatControl = new ChatControl;
			chatControl->window = window;
			chatControl->dateFont = CreateFont(14, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");
			chatControl->chat = NULL;
			chatControl->chatSizeY = 0;

			SetWindowLongPtr(window, 0, reinterpret_cast<LONG>(chatControl));
			ShowScrollBar(window, SB_VERT, FALSE);
		} break;
		case WM_CHATCONTROL_SETCHAT:
		{
			chatControl->chat = reinterpret_cast<WhatsappChat *>(lParam);
			chatControl->chatSizeY = 0;
			SetScrollPos(window, SB_VERT, 0, TRUE);
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
		case WM_VSCROLL:
		{
			return onScroll(*chatControl, wParam);
		};
		case WM_NCDESTROY:
		{
			DeleteObject(chatControl->dateFont);
			delete chatControl;
		} break;
	}

	return DefWindowProc(window, message, wParam, lParam);
}
