#pragma once

#include <vector>
#include <windows.h>

class ChatControlMessageElement;
class ImageDecoder;
class Smileys;
class WhatsappMessage;

std::string formatAudio(WhatsappMessage& message);

class ChatControlMessage
{
private:
	WhatsappMessage& message;
	int width;
	std::vector<ChatControlMessageElement *> elements;
	Smileys &smileys;

	void addText(std::string message);
	void addThumbnail(unsigned char *data, int size, ImageDecoder& imageDecoder);

public:
	ChatControlMessage(WhatsappMessage &message, int width, ImageDecoder& imageDecoder, Smileys &smileys);
	~ChatControlMessage();

	int calculateHeight();
	void render(HDC deviceContext, int x, int y, int clientHeight);

	int getWidth();
	void updateWidth(HWND window, int width);

	WhatsappMessage& getMessage();
};
