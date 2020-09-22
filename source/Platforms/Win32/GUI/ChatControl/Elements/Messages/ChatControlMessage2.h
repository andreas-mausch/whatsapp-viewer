#pragma once

#include <vector>
#include <windows.h>
#include "ChatControlMessage.h"

class ChatControlMessageElement;
class ImageDecoder;
class Smileys;
class WhatsappMessage;

std::string formatAudio(WhatsappMessage& message);

class ChatControlMessage2 : public ChatControlMessage
{
private:
	std::vector<ChatControlMessageElement *> elements;
	Smileys &smileys;

	void addText(std::string message);
	void addThumbnail(unsigned char *data, int size, ImageDecoder& imageDecoder);

public:
	ChatControlMessage2(WhatsappMessage &message, int width, ImageDecoder& imageDecoder, Smileys &smileys);
	virtual ~ChatControlMessage2();

	virtual int calculateHeight();
	virtual void render(HDC deviceContext, int x, int y, int clientHeight);

};
