#pragma once

#include <vector>
#include "../../../ThreadWindows.h"

class ChatControlMessageFrame;
class Font;
class ImageDecoder;
class Lock;
class Smileys;
class WhatsappChat;

class BuildMessagesThread : public ThreadWindows
{
private:
	HWND window;
	Lock &lock;
	WhatsappChat *chat;
	std::vector<ChatControlMessageFrame *> &messages;
	Smileys &smileys;
	Font &dateFont;
	ImageDecoder &imageDecoder;

public:
	BuildMessagesThread(HWND window, Lock &lock, WhatsappChat *chat, std::vector<ChatControlMessageFrame *> &messages, Smileys &smileys, Font &dateFont, ImageDecoder &imageDecoder);
	virtual ~BuildMessagesThread();

	virtual void run();

};
