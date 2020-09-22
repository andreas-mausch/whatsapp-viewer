#pragma once

#include <string>
#include <windows.h>

#include "ChatControlMessageElement.h"

class Smileys;

class ChatControlMessageElementText : public ChatControlMessageElement
{
private:
	std::string text;
	WCHAR *wcharText;

public:
	ChatControlMessageElementText(const std::string &text);
	virtual ~ChatControlMessageElementText();

	void calculateHeight(int width);
	void render(HDC deviceContext, int x, int y, int clientHeight, Smileys &smileys);
};
