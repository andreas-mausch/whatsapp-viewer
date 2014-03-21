#pragma once

#include <vector>

class WhatsappChat;
class WhatsappMessage;

#define WM_CHATCONTROL_SETCHAT (WM_USER+0)

void registerChatControl();
LRESULT CALLBACK ChatControlCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam);

class ChatControlMessage
{
private:
	WCHAR *wcharText;
	WCHAR *wcharDate;
	int height;
	WhatsappMessage &message;
public:
	ChatControlMessage(WhatsappMessage &message);
	~ChatControlMessage();
	WhatsappMessage &getMessage();
	WCHAR *getText();
	WCHAR *getDateText();
	int getHeight();
	void setHeight(int height);
};

struct ChatControl
{
	HWND window;
	HGDIOBJ dateFont;
	WhatsappChat *chat;
	bool scrollInfoDetermined;
	std::vector<ChatControlMessage *> messages;
};
