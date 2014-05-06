#pragma once

#include <vector>
#include "../../../ThreadWindows.h"

class ChatControlMessageFrame;
class Lock;

class ResizeMessagesThread : public ThreadWindows
{
private:
	HWND window;
	Lock &lock;
	std::vector<ChatControlMessageFrame *> &messages;
	int totalMessagesHeight;

	void resizeMessageWidths();

public:
	ResizeMessagesThread(HWND window, Lock &lock, std::vector<ChatControlMessageFrame *> &messages);
	virtual ~ResizeMessagesThread();

	virtual void run();

};
