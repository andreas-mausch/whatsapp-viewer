#pragma once

#include <vector>
#include "../../../ThreadWindows.h"

class ChatControlElement;
class Lock;

class ResizeMessagesThread : public ThreadWindows
{
private:
	HWND window;
	Lock &lock;
	std::vector<ChatControlElement *> &elements;
	int totalMessagesHeight;

	void resizeMessageWidths();

public:
	ResizeMessagesThread(HWND window, Lock &lock, std::vector<ChatControlElement *> &elements);
	virtual ~ResizeMessagesThread();

	virtual void run();

};
