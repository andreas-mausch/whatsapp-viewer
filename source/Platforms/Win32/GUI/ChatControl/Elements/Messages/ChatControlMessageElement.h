#pragma once

#include <windows.h>

class Smileys;

class ChatControlMessageElement
{
protected:
	int height;

public:
	ChatControlMessageElement();
	virtual ~ChatControlMessageElement();

	int getHeight() const;

	virtual void calculateHeight(int width) = 0;
	virtual void render(HDC deviceContext, int x, int y, int clientHeight, Smileys &smileys) = 0;
};
