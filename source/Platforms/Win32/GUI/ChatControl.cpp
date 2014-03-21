#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "ChatControl.h"
#include "ChatControlMessage.h"
#include "../StringHelper.h"
#include "../../../WhatsApp/Chat.h"
#include "../../../WhatsApp/Message.h"

ChatControl::ChatControl(HWND window)
{
	this->window = window;
	dateFont = CreateFont(13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");
	chat = NULL;
}

ChatControl::~ChatControl()
{
	clearMessages();
}

void ChatControl::registerChatControl()
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

void ChatControl::buildMessages()
{
	clearMessages();

	if (chat != NULL)
	{
		std::vector<WhatsappMessage *> &messages = chat->getMessages();
		for (std::vector<WhatsappMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
		{
			WhatsappMessage &message = **it;
			ChatControlMessage *chatControlMessage = new ChatControlMessage(message);
			this->messages.push_back(chatControlMessage);
		}

		calculateMessageHeights();
		calculateScrollInfo();
	}
}

void ChatControl::clearMessages()
{
	for (std::vector<ChatControlMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		delete *it;
	}
	messages.clear();
}

void ChatControl::calculateMessageHeights()
{
	for (std::vector<ChatControlMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		ChatControlMessage &message = **it;
		message.calculateHeight(window, dateFont);
	}
}

void ChatControl::calculateScrollInfo()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	int y = 40;
	for (std::vector<ChatControlMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		ChatControlMessage &message = **it;
		y += 8 + message.getHeight();
	}

	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_PAGE | SIF_RANGE;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = y;
	scrollInfo.nPage = clientRect.bottom;

	SetScrollInfo(window, SB_VERT, &scrollInfo, TRUE);
}

void ChatControl::deleteBackbuffer()
{
	DeleteObject(backbufferBitmap);
	backbufferBitmap = NULL;

	DeleteDC(backbuffer);
	backbuffer = NULL;
}

void ChatControl::createBackbuffer()
{
	deleteBackbuffer();

	RECT clientRect;
	GetClientRect(window, &clientRect);

	HDC deviceContext = GetDC(window);

	backbuffer = CreateCompatibleDC(deviceContext);
	backbufferBitmap = CreateCompatibleBitmap(deviceContext, clientRect.right, clientRect.bottom);
	SelectObject(backbuffer, backbufferBitmap);

	ReleaseDC(window, deviceContext);
}

LRESULT ChatControl::onPaint()
{
//	LONGLONG frequency, start, end;
//	QueryPerformanceFrequency((LARGE_INTEGER*)&frequency);
//	QueryPerformanceCounter((LARGE_INTEGER*)&start);

	HDC deviceContext;
	PAINTSTRUCT paint;

	deviceContext = BeginPaint(window, &paint);
	HANDLE oldFont = SelectObject(backbuffer, GetStockObject(DEFAULT_GUI_FONT));
	SetTextColor(backbuffer, RGB(0, 0, 0));
	SetBkColor(backbuffer, RGB(230, 230, 210));

	RECT clientRect;
	GetClientRect(window, &clientRect);

	HBRUSH brush = CreateSolidBrush(RGB(230, 230, 210));
	FillRect(backbuffer, &clientRect, brush);
	DeleteObject(brush);

	int scrollPosition = GetScrollPos(window, SB_VERT);

	int y = 10;
	if (y - scrollPosition > 0)
	{
		std::string text = "WhatsApp Chat";
		if (chat != NULL)
		{
			text += " (";
			text += chat->getKey();

			if (chat->getSubject().length() > 0)
			{
				text += "; ";
				text += chat->getSubject();
			}

			text += ")";
		}

		WCHAR *wcharText = buildWcharString(text);
		RECT titleRect = { 10, 10, clientRect.right, clientRect.bottom };
		DrawText(backbuffer, wcharText, text.length(), &titleRect, 0);
		delete[] wcharText;
	}

	y += 15;
	if (y - scrollPosition)
	{
		MoveToEx(backbuffer, 10, y - scrollPosition, NULL);
		LineTo(backbuffer, clientRect.right - 10, y - scrollPosition);
	}

	y += 15;

	if (chat != NULL)
	{
		for (std::vector<ChatControlMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
		{
			ChatControlMessage &message = **it;

			if (y + message.getHeight() - scrollPosition > 0)
			{
				drawMessage(message, backbuffer, y - scrollPosition, clientRect.right, dateFont);
			}
			y += message.getHeight();
			y += 8;

			if (y - scrollPosition > clientRect.bottom)
			{
				break;
			}
		}
	}

	SelectObject(backbuffer, oldFont);

	BitBlt(deviceContext, 0, 0, clientRect.right, clientRect.bottom, backbuffer, 0, 0, SRCCOPY);
	EndPaint(window, &paint);

//	QueryPerformanceCounter((LARGE_INTEGER*)&end);
//	double dTimeDiff = (((double)(end-start))/((double)frequency));

	return 0;
}

void ChatControl::drawMessage(ChatControlMessage &message, HDC deviceContext, int y, int clientRectWidth, HGDIOBJ dateFont)
{
	int gap = 40;
	int left = 10;
	int right = clientRectWidth - gap - 10;
	int color = RGB(230, 230, 240);

	if (message.getMessage().isFromMe())
	{
		left = 10 + gap;
		right = clientRectWidth - 10;
		color = RGB(190, 240, 150);
	}

	WCHAR *wcharText = message.getText();
	WCHAR *wcharDate = message.getDateText();

	SetBkColor(deviceContext, color);
	SetTextColor(deviceContext, RGB(0, 0, 0));
	RECT textRect = { left, y, right, y };
	int height = 0;

	DrawText(deviceContext, wcharText, -1, &textRect, DT_CALCRECT | DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);
	textRect.right = right;
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

	DrawText(deviceContext, wcharText, -1, &textRect, DT_WORDBREAK | DT_END_ELLIPSIS | DT_MODIFYSTRING);

	oldFont = SelectObject(deviceContext, dateFont);
	SetTextColor(deviceContext, RGB(110, 110, 110));
	DrawText(deviceContext, wcharDate, -1, &dateRect, DT_WORDBREAK | DT_RIGHT);
	SelectObject(deviceContext, oldFont);
}

void ChatControl::redraw()
{
	InvalidateRect(window, NULL, FALSE);
	UpdateWindow(window);
}

void ChatControl::scroll(int newPosition)
{
	int previousPosition = GetScrollPos(window, SB_VERT);

	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = newPosition;

	SetScrollInfo(window, SB_VERT, &scrollInfo, TRUE);
	GetScrollInfo(window, SB_VERT, &scrollInfo);

	if (scrollInfo.nPos != previousPosition)
	{
		// ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
		// UpdateWindow (hwnd);
		redraw();
	}
}

LRESULT ChatControl::onScroll(WPARAM wParam)
{
	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_ALL;
	GetScrollInfo(window, SB_VERT, &scrollInfo);

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

	scroll(scrollInfo.nPos);
	return 0;
}

LRESULT ChatControl::onMousewheel(int delta)
{
	int previousPosition = GetScrollPos(window, SB_VERT);
	scroll(previousPosition - delta / WHEEL_DELTA * 60);

	return 0;
}

LRESULT CALLBACK ChatControl::ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ChatControl *chatControl = reinterpret_cast<ChatControl *>(GetWindowLongPtr(window, 0));

	switch (message)
	{
		case WM_NCCREATE:
		{
			RECT rect;
			GetClientRect(window, &rect);

			chatControl = new ChatControl(window);

			SetWindowLongPtr(window, 0, reinterpret_cast<LONG>(chatControl));
			ShowScrollBar(window, SB_VERT, FALSE);
		} break;
		case WM_CREATE:
		{
			chatControl->createBackbuffer();
		} break;
		case WM_CHATCONTROL_SETCHAT:
		{
			chatControl->chat = reinterpret_cast<WhatsappChat *>(lParam);
			SetScrollPos(window, SB_VERT, 0, TRUE);
			chatControl->buildMessages();
			chatControl->redraw();
		} break;
		case WM_PAINT:
		{
			return chatControl->onPaint();
		} break;
		case WM_ERASEBKGND:
		{
			return 1;
		} break;
		case WM_VSCROLL:
		{
			return chatControl->onScroll(wParam);
		} break;
		case WM_MOUSEACTIVATE:
		{
			SetFocus(window);
			return MA_ACTIVATE;
		} break;
		case WM_MOUSEWHEEL:
		{
			return chatControl->onMousewheel(GET_WHEEL_DELTA_WPARAM(wParam));
		} break;
		case WM_SIZE:
		{
			chatControl->createBackbuffer();
			chatControl->buildMessages();
		} break;
		case WM_NCDESTROY:
		{
			DeleteObject(chatControl->dateFont);
			delete chatControl;
		} break;
	}

	return DefWindowProc(window, message, wParam, lParam);
}
