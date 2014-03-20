#pragma once

class WhatsappChat;

#define WM_CHATCONTROL_SETCHAT (WM_USER+0)

void registerChatControl();
LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

struct ChatControl
{
	WhatsappChat *chat;
};
