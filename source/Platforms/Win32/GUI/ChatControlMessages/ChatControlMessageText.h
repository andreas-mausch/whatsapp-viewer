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

public:
	ChatControlMessageText(WhatsappMessage &message, int width, Smileys &smileys);
	virtual ~ChatControlMessageText();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
