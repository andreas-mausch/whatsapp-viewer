#pragma once

#include <vector>
#include "ChatControlMessage.h"

class ChatControlMessageTextElement;
class Smileys;
class WhatsappMessage;

class ChatControlMessageText : public ChatControlMessage
{
private:
	std::vector<ChatControlMessageTextElement *> elements;
	Smileys &smileys;

	void splitMessage(WhatsappMessage &message);

public:
	ChatControlMessageText(WhatsappMessage &message, int width, Smileys &smileys);
	virtual ~ChatControlMessageText();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
