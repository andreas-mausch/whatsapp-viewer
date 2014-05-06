#include <windows.h>

#include "ResizeMessagesThread.h"
#include "../ChatControl.h"
#include "../ChatControlMessageFrame.h"
#include "../../../../../Synchronization/Lock.h"

ResizeMessagesThread::ResizeMessagesThread(HWND window, Lock &lock, std::vector<ChatControlMessageFrame *> &messages)
	: window(window), lock(lock), messages(messages), totalMessagesHeight(-1)
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
	int gap = 40;
	int width = clientRect.right - clientRect.left - 20 - gap;

	for (std::vector<ChatControlMessageFrame *>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		if (!running)
		{
			return;
		}

		ChatControlMessageFrame &messageFrame = **it;
		messageFrame.updateWidth(window, width);
		y += 8 + messageFrame.getHeight();
	}

	totalMessagesHeight = y;
}
