#pragma once

#include "ChatControlMessage.h"

class ChatControlMessageTextElement;
class Smileys;
class WhatsappMessage;

std::string formatAudio(WhatsappMessage &message);

class ChatControlMessageAudio : public ChatControlMessage
{
private:
	ChatControlMessageTextElement *textElement;
	Smileys &smileys;

public:
	ChatControlMessageAudio(WhatsappMessage &message, int width, Smileys &smileys);
	virtual ~ChatControlMessageAudio();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
