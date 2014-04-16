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
	Smileys &smileys;

	void splitMessage(WhatsappMessage &message);
	bool isSmiley(int character);

	virtual int calculateHeightInner();
	virtual void renderInner(HDC deviceContext, int x, int y, int clientHeight);

public:
	ChatControlMessageText(WhatsappMessage &message, int width, int color, HFONT dateFont, Smileys &smileys);
	virtual ~ChatControlMessageText();

};
