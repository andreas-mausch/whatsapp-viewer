#include <windows.h>
#include <commctrl.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>

#include "ChatControl.h"
#include "ChatControlElement.h"
#include "Threads/BuildMessagesThread.h"
#include "Threads/LoadingAnimationThread.h"
#include "Threads/ResizeMessagesThread.h"
#include "../AnimatedGif.h"
#include "../DrawText.h"
#include "../Smileys.h"
#include "../../Counter.h"
#include "../../ImageDecoder.h"
#include "../../StringHelper.h"
#include "../../Objects/Brush.h"
#include "../../Objects/Font.h"
#include "../../../../Exceptions/Exception.h"
#include "../../../../Libraries/UTF8/utf8.h"
#include "../../../../Synchronization/Locked.h"
#include "../../../../WhatsApp/Chat.h"
#include "../../../../WhatsApp/Message.h"
#include "../../../../Log.h"
#include "../../../../VectorUtils.h"
#include "../../../../../resources/resource.h"

ChatControl::ChatControl(HWND window)
	: imageDecoder(imageDecoder), buildMessagesThread(NULL),
	resizeMessagesThread(NULL), loadingAnimationThread(NULL),
	window(window)
{
	imageDecoder = new ImageDecoder();
	smileys = new Smileys(*imageDecoder);
	loadingAnimation = new AnimatedGif(L"IDR_LOADING", L"GIF", *imageDecoder);
	dateFont = new Font(CreateFont(13, 0, 0, 0, FW_NORMAL, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Courier New"));
	chat = NULL;
	shouldResizeMessages = true;
	painting = false;
	totalMessagesHeight = 0;
}

ChatControl::~ChatControl()
{
	stopResizingMessages();
	stopBuildingMessages();
	stopLoadingAnimation();

	clearVector(elements);
	delete dateFont;
	delete loadingAnimation;
	delete smileys;
	delete imageDecoder;
}

void ChatControl::registerControl()
{
	WNDCLASSEX windowClass;
	memset(&windowClass, 0, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.lpszClassName = L"ChatControl";
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(GetStockObject(WHITE_BRUSH));
	windowClass.style = 0;
	windowClass.lpfnWndProc = callback;
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.cbWndExtra = sizeof(ChatControl *);
	RegisterClassEx(&windowClass);
}

void ChatControl::setChat(WhatsappChat *chat)
{
	deleteBackbuffer();
	this->chat = chat;
	startBuildingMessages();

	if (chat)
	{
		// assume a value where vertical scrollbar is required
		totalMessagesHeight = 10000;
	}
	else
	{
		totalMessagesHeight = 0;
	}
	scroll(0, false);
	calculateScrollInfo();

	redraw();
}

void ChatControl::startBuildingMessages()
{
	painting = false;
	stopResizingMessages();
	stopBuildingMessages();
	buildMessagesThread = new BuildMessagesThread(window, lock, chat, elements, *smileys, *dateFont, *imageDecoder);
	buildMessagesThread->start();
}

void ChatControl::stopBuildingMessages()
{
	if (buildMessagesThread)
	{
		interruptJoinAndDeleteThread(*buildMessagesThread);
		buildMessagesThread = NULL;
	}
}

void ChatControl::startResizingMessages()
{
	stopResizingMessages();
	painting = false;

	if (!buildMessagesThread)
	{
		if (chat)
		{
			resizeMessagesThread = new ResizeMessagesThread(window, lock, elements);
			resizeMessagesThread->start();
		}
		else
		{
			PostMessage(window, WM_CHATCONTROL, CHAT_CONTROL_RESIZING_MESSAGES_FINISHED, totalMessagesHeight);
		}
	}
}

void ChatControl::stopResizingMessages()
{
	if (resizeMessagesThread)
	{
		interruptJoinAndDeleteThread(*resizeMessagesThread);
		resizeMessagesThread = NULL;
	}
}

void ChatControl::startLoadingAnimation()
{
	stopLoadingAnimation();
	loadingAnimationThread = new LoadingAnimationThread(window, *loadingAnimation);
	loadingAnimationThread->start();
}

void ChatControl::stopLoadingAnimation()
{
	if (loadingAnimationThread)
	{
		interruptJoinAndDeleteThread(*loadingAnimationThread);
		loadingAnimationThread = NULL;
	}
}

void ChatControl::interruptJoinAndDeleteThread(Thread &thread)
{
	thread.interrupt();
	thread.join();
	delete &thread;
}

void ChatControl::calculateScrollInfo()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_PAGE | SIF_RANGE;
	scrollInfo.nMin = 0;
	scrollInfo.nMax = totalMessagesHeight;
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

	paintBackbuffer();
}

void ChatControl::paintBackbuffer()
{
	COLORREF backgroundColor = RGB(228, 228, 228);

	HANDLE oldFont = SelectObject(backbuffer, GetStockObject(DEFAULT_GUI_FONT));
	SetTextColor(backbuffer, RGB(0, 0, 0));
	SetBkColor(backbuffer, backgroundColor);

	RECT clientRect;
	GetClientRect(window, &clientRect);

	Brush brush(CreateSolidBrush(backgroundColor));
	FillRect(backbuffer, &clientRect, brush.get());

	if (lock.tryLock())
	{
		painting = true;
		int scrollPosition = GetScrollPos(window, SB_VERT);

		int y = 10;

		std::stringstream text;
		text << "WhatsApp Chat";

		if (chat != NULL)
		{
			text << " (" << chat->getDisplayName();

			if (chat->getKey() != chat->getDisplayName())
			{
				text << "; " << chat->getKey();
			}

			if (chat->getSubject().length() > 0)
			{
				text << "; " << chat->getSubject();
			}

			text << ")";
		}

		WCHAR *wcharText = buildWcharString(text.str());
		int height = calculateDrawTextHeight(backbuffer, wcharText, clientRect.right - 20, static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT)));

		if (y - scrollPosition > 0)
		{
			drawText(backbuffer, wcharText, 10, 10, clientRect.right - 20);
		}

		delete[] wcharText;

		y += height;
		if (y - scrollPosition)
		{
			MoveToEx(backbuffer, 10, y - scrollPosition, NULL);
			LineTo(backbuffer, clientRect.right - 10, y - scrollPosition);
		}

		y += 15;

		if (chat != NULL)
		{
			for (std::vector<ChatControlElement *>::iterator it = elements.begin(); it != elements.end(); ++it)
			{
				if (!painting)
				{
					break;
				}

				ChatControlElement &element = **it;

				if (y + element.getHeight() - scrollPosition > 0)
				{
					element.render(backbuffer, y - scrollPosition, clientRect.bottom);
				}
				y += element.getHeight();
				y += 8;

				if (y - scrollPosition > clientRect.bottom)
				{
					break;
				}
			}
		}

		painting = false;
		lock.unlock();
	}
	else
	{
		if (!loadingAnimationThread)
		{
			startLoadingAnimation();
		}
	}

	SelectObject(backbuffer, oldFont);
}

LRESULT ChatControl::onPaint()
{
	if (!backbuffer)
	{
		createBackbuffer();
	}

	HDC deviceContext;
	PAINTSTRUCT paint;

	RECT clientRect;
	GetClientRect(window, &clientRect);

	deviceContext = BeginPaint(window, &paint);
	BitBlt(deviceContext, 0, 0, clientRect.right, clientRect.bottom, backbuffer, 0, 0, SRCCOPY);
	EndPaint(window, &paint);

	return 0;
}

void ChatControl::redraw()
{
	InvalidateRect(window, NULL, FALSE);
	UpdateWindow(window);
}

void ChatControl::scroll(int newPosition, bool redraw)
{
	int previousPosition = GetScrollPos(window, SB_VERT);

	SCROLLINFO scrollInfo;
	memset(&scrollInfo, 0, sizeof(SCROLLINFO));
	scrollInfo.cbSize = sizeof(SCROLLINFO);
	scrollInfo.fMask = SIF_POS;
	scrollInfo.nPos = newPosition;

	SetScrollInfo(window, SB_VERT, &scrollInfo, redraw);
	GetScrollInfo(window, SB_VERT, &scrollInfo);

	if (redraw && scrollInfo.nPos != previousPosition)
	{
		// ScrollWindow(hwnd, 0, yChar * (yPos - si.nPos), NULL, NULL);
		// UpdateWindow (hwnd);
		paintBackbuffer();
		ChatControl::redraw();
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

	scroll(scrollInfo.nPos, true);
	return 0;
}

LRESULT ChatControl::onMousewheel(int delta)
{
	int previousPosition = GetScrollPos(window, SB_VERT);
	scroll(previousPosition - delta / WHEEL_DELTA * 60, true);

	return 0;
}

LRESULT CALLBACK ChatControl::callback(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
{
	ChatControl *control = reinterpret_cast<ChatControl *>(GetWindowLongPtr(window, 0));

	try
	{
		switch (message)
		{
			case WM_NCCREATE:
			{
				try
				{
					control = new ChatControl(window);
				}
				catch (Exception &exception)
				{
					std::wstring cause = strtowstr(exception.getCause());
					MessageBox(NULL, cause.c_str(), L"Error", MB_OK | MB_ICONERROR);
					return FALSE;
				}

				SetWindowLongPtr(window, 0, reinterpret_cast<LONG>(control));
				ShowScrollBar(window, SB_VERT, FALSE);
			} break;
			case WM_NCDESTROY:
			{
				delete control;
			} break;
			case WM_CREATE:
			{
				if (!control)
				{
					return -1;
				}

				control->createBackbuffer();
			} break;
			case WM_PAINT:
			{
				return control->onPaint();
			} break;
			case WM_ERASEBKGND:
			{
				return 1;
			} break;
			case WM_VSCROLL:
			{
				return control->onScroll(wParam);
			} break;
			case WM_MOUSEACTIVATE:
			{
				SetFocus(window);
				return MA_ACTIVATE;
			} break;
			case WM_MOUSEWHEEL:
			{
				return control->onMousewheel(GET_WHEEL_DELTA_WPARAM(wParam));
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
			case WM_CHATCONTROL:
			{
				switch (wParam)
				{
					case CHAT_CONTROL_SETCHAT:
					{
						control->setChat(reinterpret_cast<WhatsappChat *>(lParam));
					} break;
					case CHAT_CONTROL_START_RESIZING_MESSAGES:
					{
						control->shouldResizeMessages = true;
					} break;
					case CHAT_CONTROL_STOP_RESIZING_MESSAGES:
					{
						control->shouldResizeMessages = false;
					} break;
					case CHAT_CONTROL_REDRAW:
					{
						control->startResizingMessages();
						control->createBackbuffer();
						control->redraw();
					} break;
					case CHAT_CONTROL_RESIZING_MESSAGES_FINISHED:
					{
						control->stopResizingMessages();
						control->totalMessagesHeight = lParam;
						control->stopLoadingAnimation();
						control->calculateScrollInfo();
						control->createBackbuffer();
						control->redraw();
					} break;
					case CHAT_CONTROL_BUILDING_MESSAGES_FINISHED:
					{
						control->stopBuildingMessages();
						control->startResizingMessages();
					} break;
				}
			} break;
			case WM_SIZE:
			{
				if (control->shouldResizeMessages)
				{
					control->startResizingMessages();
				}
				control->createBackbuffer();
				control->redraw();
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
