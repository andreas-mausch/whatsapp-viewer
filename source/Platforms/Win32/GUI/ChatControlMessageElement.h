#pragma once

#include <string>
#include <windows.h>

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

	void calculateHeight(HDC deviceContext, int left, int right);
	void render(HDC deviceContext, int y, int left, int right);

	int getHeight() const;

};
