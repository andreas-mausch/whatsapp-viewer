#pragma once

#include <string>
#include <windows.h>

class Smileys;

enum ChatControlMessageElementType
{
	CHAT_CONTROL_MESSAGE_ELEMENT_TEXT,
	CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY
};

class ChatControlMessageElement
{
private:
	int type;
	std::string text;
	WCHAR *wcharText;
	int smiley;
	int height;

public:
	ChatControlMessageElement(const std::string &text);
	ChatControlMessageElement(int smiley);
	~ChatControlMessageElement();

	void calculateHeight(HDC deviceContext, int width);
	void render(HDC deviceContext, int x, int y, int width, Smileys &smileys);

	int getHeight() const;

};
