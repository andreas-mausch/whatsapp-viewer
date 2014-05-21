#include <windows.h>

#include "ResizeMessagesThread.h"
#include "../ChatControl.h"
#include "../ChatControlElement.h"
#include "../../../../../Synchronization/Lock.h"

ResizeMessagesThread::ResizeMessagesThread(HWND window, Lock &lock, std::vector<ChatControlElement *> &elements)
	: window(window), lock(lock), elements(elements), totalMessagesHeight(-1)
{
}

ResizeMessagesThread::~ResizeMessagesThread()
{
}

void ResizeMessagesThread::run()
{
	if (!lock.tryLockWhile(running))
	{
		return;
	}

	resizeMessageWidths();

	if (running)
	{
		PostMessage(window, WM_CHATCONTROL, CHAT_CONTROL_RESIZING_MESSAGES_FINISHED, totalMessagesHeight);
	}

	lock.unlock();
}

void ResizeMessagesThread::resizeMessageWidths()
{
	RECT clientRect;
	GetClientRect(window, &clientRect);

	int y = 40;
	int width = clientRect.right - clientRect.left;

	for (std::vector<ChatControlElement *>::iterator it = elements.begin(); it != elements.end(); ++it)
	{
		if (!running)
		{
			return;
		}

		ChatControlElement &element = **it;
		element.updateWidth(window, width);
		y += 8 + element.getHeight();
	}

	totalMessagesHeight = y;
}
