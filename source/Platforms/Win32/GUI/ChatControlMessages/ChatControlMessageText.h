#pragma once

#include <vector>
#include "ChatControlMessage.h"

class ChatControlMessageElement;
class Smileys;
class WhatsappMessage;

class ChatControlMessageText : public ChatControlMessage
{
private:
	std::vector<ChatControlMessageElement *> elements;
	int height;

	Smileys &smileys;

	void splitMessage(WhatsappMessage &message);
	bool isSmiley(int character);

public:
	ChatControlMessageText(WhatsappMessage &message, int width, int color, HFONT dateFont, Smileys &smileys);
	virtual ~ChatControlMessageText();

	virtual void calculateHeight(HWND window);
	virtual int getHeight();

	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight);

};
