#pragma once

#include <string>
#include <windows.h>

#include "ChatControlMessageElement.h"

class Smileys;

class ChatControlMessageElementSmiley : public ChatControlMessageElement
{
private:
	int smiley;

public:
	ChatControlMessageElementSmiley(int smiley);
	virtual ~ChatControlMessageElementSmiley();

	void calculateHeight(int width);
	void render(HDC deviceContext, int x, int y, int clientHeight, Smileys &smileys);
};
