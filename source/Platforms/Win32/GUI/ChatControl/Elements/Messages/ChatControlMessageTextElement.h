#pragma once

#include <string>
#include <windows.h>

class Smileys;

enum ChatControlMessageTextElementType
{
	CHAT_CONTROL_MESSAGE_ELEMENT_TEXT,
	CHAT_CONTROL_MESSAGE_ELEMENT_SMILEY
};

class ChatControlMessageTextElement
{
private:
	int type;
	std::string text;
	WCHAR *wcharText;
	int smiley;
	int height;

public:
	ChatControlMessageTextElement(const std::string &text);
	ChatControlMessageTextElement(int smiley);
	~ChatControlMessageTextElement();

	void calculateHeight(int width);
	void render(HDC deviceContext, int x, int y, int width, Smileys &smileys);

	int getHeight() const;

};
