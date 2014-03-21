#pragma once

#include <vector>

class WhatsappChat;
class ChatControlMessage;

#define WM_CHATCONTROL_SETCHAT (WM_USER+0)

void registerChatControl();
LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

struct ChatControl
{
	HWND window;
	HGDIOBJ dateFont;
	WhatsappChat *chat;
	std::vector<ChatControlMessage *> messages;
};
