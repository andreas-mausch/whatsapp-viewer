#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <iostream>
#include <fstream>

#include "ChatControl.h"
#include "ChatControlMessages/ChatControlMessage.h"
#include "ChatControlMessages/ChatControlMessageImage.h"
#include "ChatControlMessages/ChatControlMessageLocation.h"
#include "ChatControlMessages/ChatControlMessageText.h"
#include "ChatControlMessages/ChatControlMessageVideo.h"
#include "DrawText.h"
#include "Smileys.h"
#include "../ImageDecoder.h"
#include "../StringHelper.h"
#include "../../../Exceptions/Exception.h"
#include "../../../WhatsApp/Chat.h"
#include "../../../WhatsApp/Message.h"
#include "../../../UTF8/utf8.h"
#include "../../../VectorUtils.h"

ChatControl::ChatControl(HWND window)
{
	imageDecoder = new ImageDecoder();
	smileys = new Smileys(*imageDecoder);
	this->window = window;
	dateFont = CreateFont(13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New");
	chat = NULL;
}

ChatControl::~ChatControl()
{
	clearMessages();
	DeleteObject(dateFont);
	delete smileys;
	delete imageDecoder;
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
			ChatControlMessage *chatControlMessage = NULL;

			int color;

			if (message.isFromMe())
			{
				color = RGB(190, 240, 150);
			}
			else
			{
				color = RGB(230, 230, 240);
			}

			switch (message.getMediaWhatsappType())
			{
				case MEDIA_WHATSAPP_TEXT:
				{
					chatControlMessage = new ChatControlMessageText(message, 0, color, dateFont, *smileys);
				} break;
				case MEDIA_WHATSAPP_IMAGE:
				{
					if (message.getRawDataSize() > 0 && message.getRawData() != NULL)
					{
						chatControlMessage = new ChatControlMessageImage(message, 0, color, dateFont, *imageDecoder);
					}
				} break;
				case MEDIA_WHATSAPP_VIDEO:
				{
					chatControlMessage = new ChatControlMessageVideo(message, 0, color, dateFont, *imageDecoder);
				} break;
				case MEDIA_WHATSAPP_LOCATION:
				{
					chatControlMessage = new ChatControlMessageLocation(message, 0, color, dateFont, *imageDecoder);
				} break;
			}

			if (chatControlMessage != NULL)
			{
				this->messages.push_back(chatControlMessage);
			}
		}

	}

	resizeMessages();
}

void ChatControl::resizeMessages()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	int y = 40;
	int gap = 40;
	int width = clientRect.right - clientRect.left - 20 - gap;

	for (std::vector<ChatControlMessage *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		ChatControlMessage &message = **it;
		message.updateWidth(window, width);
		y += 8 + message.getHeight();
	}

	calculateScrollInfo();
}

void ChatControl::clearMessages()
{
	clearVector(messages);
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
		drawText(backbuffer, wcharText, 10, 10, clientRect.right - 10);
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

			int x = 10;

			if (message.getMessage().isFromMe())
			{
				x += 40;
			}

			if (y + message.getHeight() - scrollPosition > 0)
			{
				message.render(backbuffer, x, y - scrollPosition, clientRect.bottom);
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

	return 0;
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
		case WM_KEYDOWN:
		{
			switch (wParam)
			{
				case VK_DOWN:
				{
					SendMessage(window, WM_VSCROLL, MAKELONG(SB_LINEDOWN, 0), 0);
					return 0;
				} break;
				case VK_UP:
				{
					SendMessage(window, WM_VSCROLL, MAKELONG(SB_LINEUP, 0), 0);
					return 0;
				} break;
				case VK_HOME:
				{
					SendMessage(window, WM_VSCROLL, MAKELONG(SB_TOP, 0), 0);
					return 0;
				} break;
				case VK_END:
				{
					SendMessage(window, WM_VSCROLL, MAKELONG(SB_BOTTOM, 0), 0);
					return 0;
				} break;
				case VK_PRIOR:
				{
					SendMessage(window, WM_VSCROLL, MAKELONG(SB_PAGEUP, 0), 0);
					return 0;
				} break;
				case VK_NEXT:
				{
					SendMessage(window, WM_VSCROLL, MAKELONG(SB_PAGEDOWN, 0), 0);
					return 0;
				} break;
			}
         } break;
		case WM_GETDLGCODE:
		{
			return DLGC_WANTARROWS;
		} break;
		case WM_SIZE:
		{
			chatControl->createBackbuffer();
			chatControl->resizeMessages();
		} break;
		case WM_NCDESTROY:
		{
			delete chatControl;
		} break;
	}

	return DefWindowProc(window, message, wParam, lParam);
}
